*This project has been created as part of the 42 curriculum by acobbaer.*

# push\_swap

## Description

`push_swap` is a sorting project from the 42 common core. The program
receives a list of integers as arguments and has to sort them using two
stacks, called A and B, and a limited set of instructions. It does not
print the sorted numbers. It prints the list of instructions that sorts
them, and the goal is to use as few instructions as possible.

At the start, all numbers are in stack A (the first argument is on top)
and B is empty. At the end, A must be sorted in ascending order and B
must be empty.

The allowed instructions are:

- `sa`, `sb`, `ss`: swap the first two elements of A, of B, or of both
- `pa`, `pb`: take the top element of one stack and put it on top of
  the other
- `ra`, `rb`, `rr`: rotate A, B, or both (the first element becomes
  the last)
- `rra`, `rrb`, `rrr`: reverse rotate A, B, or both (the last element
  becomes the first)

If the input is invalid (not a number, a duplicate, or a value outside
the int range), the program prints `Error` on the standard error output.

### Algorithm

I used the Turk algorithm for inputs bigger than 5 numbers. Smaller
inputs have their own simple cases:

- 2 numbers: one `sa` if needed.
- 3 numbers: move the biggest number to the bottom, then swap the top
  two if needed. This takes at most 2 moves.
- 4 or 5 numbers: push the smallest numbers to B until 3 are left, sort
  those 3, then push everything back.

The Turk algorithm works in four steps:

1. Push the first two numbers of A to B, so that B has something to
   compare against.
2. While A has more than 3 numbers, calculate a cost for every number
   in A and push the cheapest one to B. Each number in A has a target
   in B: the closest smaller number, or the biggest number of B if there
   is no smaller one. The cost is the number of rotations needed to
   bring the number to the top of A plus the rotations needed to bring
   its target to the top of B. Using `ra` or `rra` depends on whether
   the node is in the upper or lower half of the stack. This keeps B
   sorted in descending order.
3. Sort the last 3 numbers left in A.
4. Push every number from B back to A. For each one, rotate A so the
   closest bigger number is on top, then push. When all numbers are
   back, rotate A until the smallest number is on top.

Each stack is a doubly linked list. Every node stores its value, its
position in the stack, whether it is above the median, its target node
and its cost. These values are recalculated before each move. A linked
list makes push, swap and rotate simple pointer changes, without
shifting or resizing anything.

Results measured on random inputs:

| Numbers | Average moves | Limit for full marks |
|---------|---------------|----------------------|
| 3       | 2 at most     | 3                    |
| 5       | 10 at most    | 12                   |
| 100     | about 630     | 700                  |
| 500     | about 5900    | 5500                 |

The result for 500 numbers could be improved by combining rotations of
A and B into `rr` or `rrr` when both stacks need to rotate in the same
direction.

### What I learned

- Working with doubly linked lists and pointers to pointers
- Designing a greedy algorithm based on a cost calculation
- Parsing and validating input, including int overflow and duplicates
- Managing memory correctly, including freeing everything on error paths
- Checking for memory problems with `leaks`, AddressSanitizer and
  malloc failure tests

## Instructions

### Compilation

```
make        # build push_swap
make clean  # remove object files
make fclean # remove object files and the binary
make re     # rebuild everything
```

### Usage

The numbers can be given as separate arguments or as a single string:

```
./push_swap 3 2 1
./push_swap "4 67 3 87 23"
```

To count the moves and check that the result is sorted:

```
ARG=$(python3 -c \
  "import random; print(*random.sample(range(-1000, 1000), 100))")
./push_swap $ARG | wc -l
./push_swap $ARG | python3 tests/checker.py $ARG
```

The checker prints `OK` followed by the number of moves, or `KO` if the
stack is not sorted.

### Tests

```
make test          # full test suite
QUICK=1 make test  # faster version with fewer random runs
```

The test script checks:

- Error handling on invalid input (output must be `Error` on stderr
  only)
- Memory leaks, using the macOS `leaks` tool
- Memory errors, using a build with AddressSanitizer
- Malloc failures: every malloc call is made to fail once, and the
  program must not crash or leak
- Correctness and move count on all orders of 3 and 5 numbers, and on
  random lists of 100 and 500

The leak checks need macOS. On Linux,
`valgrind --leak-check=full ./push_swap ...` can be used instead.

### Visualizer

```
make visualize                           # 100 random numbers
python3 visualizer/visualize.py -n 500   # choose how many random numbers
python3 visualizer/visualize.py 5 2 9 1  # use your own numbers
```

The visualizer runs `push_swap` and shows both stacks being sorted step
by step. You can choose how many numbers to generate or type your own,
then play, pause, step forward or backward and change the speed. It
requires Python 3 with tkinter.

The `tests` and `visualizer` folders are development tools and are not
part of the graded project.

## Resources

- [Push Swap: The least amount of moves with two stacks][turk], the
  article by A. Yigit Ogun that describes the Turk algorithm
- [Stack (abstract data type)][stack], Wikipedia
- [Doubly linked list][list], Wikipedia
- [AddressSanitizer documentation][asan], Clang
- `man leaks` and `man malloc` on macOS

[turk]: https://medium.com/@ayogun/push-swap-c1f5d2d41e97
[stack]: https://en.wikipedia.org/wiki/Stack%20%28abstract%20data%20type%29
[list]: https://en.wikipedia.org/wiki/Doubly%20linked%20list
[asan]: https://clang.llvm.org/docs/AddressSanitizer.html

### Use of AI

I used Claude (Anthropic) as an assistant during this project for the
following tasks:

- Reviewing the code for memory leaks and bugs. It found three problems
  that I then fixed: very large numbers were not detected as overflow,
  `Error` was printed on stdout instead of stderr, and `ft_split` leaked
  memory when a malloc failed.
- Writing the test script in the `tests` folder (leak checks, sanitizer
  build, malloc failure tests and the checker).
- Writing the visualizer in the `visualizer` folder.
- Helping write and format this README.

The parsing, the stack operations and the sorting algorithm were written
by me.
