#!/usr/bin/env python3
"""Replays push_swap instructions from stdin on the given numbers.

Usage: ./push_swap $ARGS | python3 tests/checker.py $ARGS
Prints "OK <count>" or "KO <reason>" and exits 0 / 1.
"""
import sys


def apply(op, a, b):
    if op == "sa" or op == "ss":
        if len(a) > 1:
            a[0], a[1] = a[1], a[0]
    if op == "sb" or op == "ss":
        if len(b) > 1:
            b[0], b[1] = b[1], b[0]
    if op == "pa":
        if b:
            a.insert(0, b.pop(0))
    elif op == "pb":
        if a:
            b.insert(0, a.pop(0))
    if op == "ra" or op == "rr":
        if a:
            a.append(a.pop(0))
    if op == "rb" or op == "rr":
        if b:
            b.append(b.pop(0))
    if op == "rra" or op == "rrr":
        if a:
            a.insert(0, a.pop())
    if op == "rrb" or op == "rrr":
        if b:
            b.insert(0, b.pop())


VALID = {"sa", "sb", "ss", "pa", "pb", "ra", "rb", "rr", "rra", "rrb", "rrr"}


def main():
    nums = []
    for arg in sys.argv[1:]:
        nums.extend(int(x) for x in arg.split())
    a, b = nums[:], []
    count = 0
    for line in sys.stdin:
        op = line.rstrip("\n")
        if op not in VALID:
            print(f"KO invalid instruction {op!r}")
            return 1
        apply(op, a, b)
        count += 1
    if b or a != sorted(nums):
        print(f"KO not sorted after {count} ops")
        return 1
    print(f"OK {count}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
