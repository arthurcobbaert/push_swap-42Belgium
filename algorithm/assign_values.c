/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_values.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acobbaer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:49:07 by acobbaer          #+#    #+#             */
/*   Updated: 2026/02/06 19:33:45 by acobbaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	init_nodes_a(t_stack **stack_a, t_stack **stack_b)
{
	assign_pos(*stack_a);
	assign_pos(*stack_b);
	find_target_node(stack_a, stack_b);
	calculate_cost(*stack_a, *stack_b);
}

void	assign_index(t_stack *stack_a)
{
	int		i;
	t_stack	*node;
	t_stack	*node_to_compare;

	node = stack_a;
	if (!node)
		return ;
	while (node)
	{
		node_to_compare = stack_a;
		i = 0;
		while (node_to_compare)
		{
			if (node_to_compare->value < node->value)
				i++;
			node_to_compare = node_to_compare->next;
		}
		node->index = i;
		node = node->next;
	}
}

void	calculate_cost(t_stack *stack_a, t_stack *stack_b)
{
	int		size_a;
	int		size_b;

	if (!stack_a || !stack_b)
		return ;
	size_a = stack_size(stack_a);
	size_b = stack_size(stack_b);
	while (stack_a)
	{
		if (stack_a->above_median)
			stack_a->cost_a = stack_a->pos;
		else
			stack_a->cost_a = size_a - stack_a->pos;
		if (stack_a->target->pos <= size_b / 2)
			stack_a->cost_a += stack_a->target->pos;
		else
			stack_a->cost_a += size_b - stack_a->target->pos;
		stack_a = stack_a->next;
	}
}

void	assign_pos(t_stack *stack)
{
	int		i;
	int		median;
	t_stack	*node;

	node = stack;
	i = 0;
	if (!node)
		return ;
	median = stack_size(stack) / 2;
	while (node)
	{
		node->pos = i;
		if (node->pos <= median)
			node->above_median = true;
		else
			node->above_median = false;
		node = node->next;
		i++;
	}
}

void	find_target_node(t_stack **stack_a, t_stack **stack_b)
{
	t_stack		*curr_a;
	t_stack		*curr_b;
	t_stack		*target_node;

	curr_a = *stack_a;
	while (curr_a)
	{
		target_node = NULL;
		curr_b = *stack_b;
		while (curr_b)
		{
			if (curr_b->value < curr_a->value
				&& (!target_node || curr_b->value > target_node->value))
				target_node = curr_b;
			curr_b = curr_b->next;
		}
		if (!target_node)
			curr_a->target = find_max(*stack_b);
		else
			curr_a->target = target_node;
		curr_a = curr_a->next;
	}
}
