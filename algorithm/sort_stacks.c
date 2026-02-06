/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_stacks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acobbaer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:05:37 by acobbaer          #+#    #+#             */
/*   Updated: 2026/02/06 16:06:36 by acobbaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_stacks(t_stack **stack_a, t_stack **stack_b)
{
	int		stack_len_a;

	stack_len_a = stack_size(*stack_a);
	if (stack_len_a-- > 3 && (!is_sorted(stack_a)))
		push_b(stack_a, stack_b);
	if (stack_len_a-- > 3 && (!is_sorted(stack_a)))
		push_b(stack_a, stack_b);
	while (stack_len_a-- > 3)
	{
		init_nodes_a(stack_a, stack_b);
		move_to_b(stack_a, stack_b);
	}
	sort_tiny(stack_a);
	while (*stack_b)
	{
		init_nodes_b(stack_a, stack_b);
		move_to_a(stack_a, stack_b);
	}
	min_on_top(stack_a);
}

void	min_on_top(t_stack **stack_a)
{
	t_stack		*min_node;

	min_node = find_min(*stack_a);
	while (*stack_a != min_node)
	{
		if (min_node->above_median)
			rotate_a(stack_a);
		else
			reverse_rotate_a(stack_a);
	}
	assign_pos(*stack_a);
}

void	move_to_b(t_stack **stack_a, t_stack **stack_b)
{
	t_stack		*cheapest_node;

	if (!stack_a || !(*stack_a))
		return ;
	cheapest_node = get_cheapest(*stack_a);
	prep_for_push(stack_a, cheapest_node, 'a');
	prep_for_push(stack_b, cheapest_node->target, 'b');
	push_b(stack_a, stack_b);
}

void	prep_for_push(t_stack **stack, t_stack *node_to_top, char stack_name)
{
	if (!stack || !(*stack))
		return ;
	if (node_to_top->above_median == true)
	{
		while (*stack != node_to_top)
		{
			if (stack_name == 'a')
				rotate_a(stack);
			else
				rotate_b(stack);
		}
	}
	else if (node_to_top->above_median == false)
	{
		while (*stack != node_to_top)
		{
			if (stack_name == 'a')
				reverse_rotate_a(stack);
			else
				reverse_rotate_b(stack);
		}
	}
}

t_stack	*get_cheapest(t_stack *stack_a)
{
	t_stack		*cheapest;

	if (!stack_a)
		return (NULL);
	cheapest = stack_a;
	while (stack_a)
	{
		if (stack_a->cost_a < cheapest->cost_a)
			cheapest = stack_a;
		stack_a = stack_a->next;
	}
	return (cheapest);
}
