NAME = push_swap
CC = cc
CFLAGS = -Wall -Werror -Wextra -I.

SRCS =        parser/check_arg.c \
              parser/ft_split.c \
              operations/push.c \
              operations/rotate.c \
              operations/swap.c \
              operations/reverse_rotate.c \
              algorithm/assign_values.c \
              algorithm/free.c \
              algorithm/initialize.c \
              algorithm/utils.c \
              algorithm/push_swap.c \
              algorithm/sort_stacks.c \
              algorithm/prepare_b.c \
              main.c \

MY_OBJECTS = $(SRCS:.c=.o)
TEST_OBJS   = $(TEST_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(MY_OBJECTS)
	$(CC) $(CFLAGS) $(MY_OBJECTS) -o $(NAME)

$(TEST_NAME): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) -o $(TEST_NAME)
	@echo "Test executable created: ./test_exec"

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(MY_OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all
