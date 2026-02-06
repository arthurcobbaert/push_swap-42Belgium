/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acobbaer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 18:08:31 by acobbaer          #+#    #+#             */
/*   Updated: 2026/02/06 19:34:52 by acobbaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int argc, char **argv)
{
	t_stack		*stack_a;
	t_stack		*stack_b;
	char		**args;

	if (argc == 1 || (argc == 2 && !argv[1][0]))
		return (1);
	if (argc == 2)
		args = ft_split(argv[1], ' ');
	else
		args = argv + 1;
	if (!args || !args[0] || !is_valid_input(args))
	{
		if (argc == 2)
			free_argv(args);
		ft_putstr("Error\n");
		return (0);
	}
	stack_a = init_stack_a(args);
	stack_b = NULL;
	assign_index(stack_a);
	push_swap(&stack_a, &stack_b);
	if (argc == 2)
		free_argv(args);
	return (free_stack(&stack_a), free_stack(&stack_b), 0);
}

/*
#include <stdio.h>

void	print_stack(t_stack *stack_a, t_stack *stack_b)
{
	printf("----Stack_a----\n");
	if (!stack_a)
		printf("Stack A is empty!\n");
	while (stack_a)
	{
		printf("Value: %d | Index:%d | Pos: %d\n", 
				stack_a->value, stack_a->index, stack_a->pos);
		stack_a = stack_a->next;
	}

	printf("----Stack_b----\n");
	if (!stack_b)
		printf("Stack B is empty!\n");
	while (stack_b)
	{
		printf("Value: %d | Index:%d | Pos: %d\n", 
				stack_b->value, stack_b->index, stack_b->pos);
		stack_b = stack_b->next;
	}
} */
