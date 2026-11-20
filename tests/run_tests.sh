#!/usr/bin/env bash
# push_swap test suite: error handling, memory leaks, memory errors,
# malloc-failure paths, correctness and operation counts.
#
# Usage: tests/run_tests.sh            (full run)
#        QUICK=1 tests/run_tests.sh    (fewer random runs)

cd "$(dirname "$0")/.." || exit 1
ROOT=$(pwd)
BUILD="$ROOT/tests/build"
CHECKER="python3 $ROOT/tests/checker.py"
SRCS="$(find parser operations algorithm -name '*.c') main.c"

G='\033[32m'; R='\033[31m'; Y='\033[33m'; B='\033[1m'; N='\033[0m'
PASS=0; FAIL=0
ok()   { PASS=$((PASS + 1)); [ -n "$VERBOSE" ] && printf "  ${G}✔${N} %s\n" "$1"; return 0; }
ko()   { FAIL=$((FAIL + 1)); printf "  ${R}✘ %s${N}\n" "$1"; [ -n "$2" ] && printf "    %s\n" "$2"; }
section() { printf "\n${B}== %s ==${N}\n" "$1"; }

rand_nums() { python3 -c "import random,sys; n=int(sys.argv[1]); print(' '.join(map(str, random.sample(range(-2**31, 2**31), n))))" "$1"; }

# ---------------------------------------------------------------- build
section "Build"
mkdir -p "$BUILD"
make -s re >/dev/null 2>&1 || { ko "make failed"; exit 1; }
cp push_swap "$BUILD/push_swap"
cc -Wall -Wextra -Werror -I. -g -fsanitize=address,undefined $SRCS -o "$BUILD/push_swap_asan" \
	|| { ko "ASan build failed"; exit 1; }
cc -Wall -Wextra -Werror -g -c tests/malloc_fail.c -o "$BUILD/malloc_fail.o" \
	&& cc -Wall -Wextra -Werror -I. -g -include tests/malloc_fail.h $SRCS "$BUILD/malloc_fail.o" -o "$BUILD/push_swap_mfail" \
	|| { ko "malloc-fail build failed"; exit 1; }
make -s fclean >/dev/null
printf "  normal, AddressSanitizer and malloc-failure builds ready\n"
PS="$BUILD/push_swap"

# ---------------------------------------------------------------- cases
# Each line = arguments exactly as typed in a shell.
ERROR_CASES=(
	'a' '1 2 a' '"1 2 a"' '1 1' '"3 2 3"' '2147483648' '-2147483649'
	'99999999999999999999' '18446744073709551616 1' '"-9223372036854775809"'
	'+' '-' '"+ 1"' '1 -' '"1 2" 3' '1 ++2' '"1  2 2"' '" "' '0 -0' '1.5'
)
NO_OUTPUT_CASES=( '' '""' '42' '1 2 3' '"1 2 3 4 5"' '-2147483648 2147483647' )
VALID_CASES=( '2 1' '3 2 1' '"5 4 3 2 1"' '1 5 2 4 3' '-1 -5 0 2147483647 -2147483648 9' '+3 -4 +0 7 1 2' )

# ---------------------------------------------------------------- errors
section "Error handling (expects \"Error\\n\" on stderr, nothing on stdout)"
for c in "${ERROR_CASES[@]}"; do
	eval "set -- $c"
	out=$("$PS" "$@" 2>/dev/null); err=$("$PS" "$@" 2>&1 >/dev/null)
	if [ "$err" = "Error" ] && [ -z "$out" ]; then ok "$c"
	else ko "./push_swap $c" "stdout='$out' stderr='$err'"; fi
done
for c in "${NO_OUTPUT_CASES[@]}"; do
	eval "set -- $c"
	all=$("$PS" "$@" 2>&1)
	if [ -z "$all" ]; then ok "$c"; else ko "./push_swap $c should print nothing" "got: $all"; fi
done

# ---------------------------------------------------------------- leaks
section "Memory leaks (macOS leaks --atExit)"
leak_check() {
	local res
	res=$(leaks --atExit -q -- "$@" 2>&1 | grep -E "^Process [0-9]+: [0-9]+ leaks")
	if echo "$res" | grep -q ": 0 leaks"; then return 0; fi
	echo "${res:-leaks produced no report}"; return 1
}
LEAK_CASES=("${ERROR_CASES[@]}" "${NO_OUTPUT_CASES[@]}" "${VALID_CASES[@]}"
	"\"$(rand_nums 100)\"" "$(rand_nums 100)" "\"$(rand_nums 500)\"" "$(rand_nums 500)")
for c in "${LEAK_CASES[@]}"; do
	eval "set -- $c"
	if msg=$(leak_check "$PS" "$@"); then ok "${c:0:60}"; else ko "leak: ./push_swap ${c:0:60}" "$msg"; fi
done
printf "  checked %d inputs\n" "${#LEAK_CASES[@]}"

# ---------------------------------------------------------------- asan
section "Memory errors (AddressSanitizer + UBSan)"
ASAN_CASES=("${LEAK_CASES[@]}")
for _ in $(seq 1 "$([ -n "$QUICK" ] && echo 5 || echo 20)"); do ASAN_CASES+=("$(rand_nums $((RANDOM % 120 + 1)))"); done
for c in "${ASAN_CASES[@]}"; do
	eval "set -- $c"
	err=$("$BUILD/push_swap_asan" "$@" 2>&1 >/dev/null)
	if echo "$err" | grep -qE "AddressSanitizer|runtime error"; then
		ko "./push_swap ${c:0:60}" "$(echo "$err" | grep -m1 -E 'ERROR|runtime error')"
	else ok "${c:0:60}"; fi
done
printf "  checked %d inputs\n" "${#ASAN_CASES[@]}"

# ---------------------------------------------------------------- malloc failure
section "malloc failure paths (every malloc fails once, then leaks is checked)"
MF="$BUILD/push_swap_mfail"
for c in '5 4 3 2 1' '"5 4 3 2 1"' '"1 2 x"' "\"$(rand_nums 20)\""; do
	eval "set -- $c"
	total=$(MALLOC_COUNT=1 "$MF" "$@" 2>&1 >/dev/null | sed -n 's/MALLOC_CALLS=//p')
	[ -z "$total" ] && { ko "could not count mallocs for $c"; continue; }
	bad=0
	for n in $(seq 1 "$total"); do
		FAIL_AT=$n "$MF" "$@" >/dev/null 2>&1; code=$?
		if [ $code -ge 128 ]; then ko "crash (signal $((code - 128))) when malloc #$n fails: ${c:0:40}"; bad=1; continue; fi
		if ! msg=$(FAIL_AT=$n leak_check "$MF" "$@"); then
			ko "leak when malloc #$n fails: ${c:0:40}" "$msg"; bad=1
		fi
	done
	[ $bad -eq 0 ] && ok "$c" && printf "  ${G}✔${N} %-45s %3d mallocs, each failure path clean\n" "${c:0:45}" "$total"
done

# ---------------------------------------------------------------- correctness
section "Correctness and operation count"
check_sort() {  # args... -> prints "OK n" / "KO ..."
	"$PS" "$@" | $CHECKER "$@"
}
# every permutation of 3 and 5
for size in 3 5; do
	worst=0; bad=0
	while read -r perm; do
		res=$(check_sort $perm)
		case "$res" in OK*) n=${res#OK }; [ "$n" -gt "$worst" ] && worst=$n ;;
		*) ko "./push_swap $perm" "$res"; bad=1 ;; esac
	done < <(python3 -c "import itertools; [print(*p) for p in itertools.permutations(range($size))]")
	limit=$([ $size -eq 3 ] && echo 3 || echo 12)
	if [ $bad -eq 0 ] && [ "$worst" -le "$limit" ]; then ok "size $size"
		printf "  ${G}✔${N} all permutations of %d sorted, worst %d ops (limit %d)\n" $size "$worst" "$limit"
	elif [ $bad -eq 0 ]; then ko "size $size worst case $worst ops > $limit"; fi
done
# random benchmarks
bench() {
	local size=$1 runs=$2 limit=$3 total=0 worst=0 bad=0 res n
	for _ in $(seq 1 "$runs"); do
		nums=$(rand_nums "$size")
		res=$(check_sort $nums)
		case "$res" in OK*) n=${res#OK }; total=$((total + n)); [ "$n" -gt "$worst" ] && worst=$n ;;
		*) ko "size $size: $res"; bad=1 ;; esac
	done
	[ $bad -eq 1 ] && return
	local color=$G; [ "$worst" -gt "$limit" ] && color=$Y
	printf "  ${color}●${N} size %3d × %2d runs: avg %4d, worst %4d ops (100%% grade < %d)\n" \
		"$size" "$runs" $((total / runs)) "$worst" "$limit"
	ok "bench $size"
}
bench 100 "$([ -n "$QUICK" ] && echo 5 || echo 30)" 700
bench 500 "$([ -n "$QUICK" ] && echo 3 || echo 15)" 5500

# ---------------------------------------------------------------- summary
section "Summary"
if [ $FAIL -eq 0 ]; then printf "${G}${B}All %d checks passed${N}\n" $PASS; exit 0
else printf "${R}${B}%d failed${N}, %d passed\n" $FAIL $PASS; exit 1; fi
