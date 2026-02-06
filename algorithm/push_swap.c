/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acobbaer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:07:46 by acobbaer          #+#    #+#             */
/*   Updated: 2026/02/06 16:08:22 by acobbaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	push_swap(t_stack **stack_a, t_stack **stack_b)
{
	if (!stack_a || !*stack_a)
		return ;
	if (!is_sorted(stack_a))
	{
		assign_pos(*stack_a);
		assign_index(*stack_a);
		if (stack_size(*stack_a) == 2)
			swap_a(stack_a);
		else if (stack_size(*stack_a) == 3)
			sort_tiny(stack_a);
		else if (stack_size(*stack_a) <= 5)
			sort_five(stack_a, stack_b);
		else
			sort_stacks(stack_a, stack_b);
	}
}

void	sort_five(t_stack **stack_a, t_stack **stack_b)
{
	while (stack_size(*stack_a) > 3)
	{
		min_on_top(stack_a);
		push_b(stack_a, stack_b);
	}
	sort_tiny(stack_a);
	while (*stack_b)
		push_a(stack_b, stack_a);
	assign_pos(*stack_a);
}

void	sort_tiny(t_stack **stack_a)
{
	t_stack		*max_node;

	max_node = find_max(*stack_a);
	if (max_node == *stack_a)
		rotate_a(stack_a);
	else if (max_node == (*stack_a)->next)
		reverse_rotate_a(stack_a);
	if (!is_sorted(stack_a))
		swap_a(stack_a);
}

t_stack	*find_max(t_stack *stack)
{
	t_stack		*biggest_node;

	biggest_node = stack;
	while (stack)
	{
		if (stack->value > biggest_node->value)
			biggest_node = stack;
		stack = stack->next;
	}
	return (biggest_node);
}

t_stack	*find_min(t_stack *stack)
{
	t_stack		*smallest_node;

	if (!stack)
		return (NULL);
	smallest_node = stack;
	while (stack)
	{
		if (stack->value < smallest_node->value)
			smallest_node = stack;
		stack = stack->next;
	}
	return (smallest_node);
}
