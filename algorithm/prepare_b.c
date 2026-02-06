/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acobbaer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:08:34 by acobbaer          #+#    #+#             */
/*   Updated: 2026/02/06 19:32:03 by acobbaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	init_nodes_b(t_stack **stack_a, t_stack **stack_b)
{
	assign_pos(*stack_a);
	assign_pos(*stack_b);
	find_target_node_b(stack_a, stack_b);
	calculate_cost(*stack_a, *stack_b);
}

void	find_target_node_b(t_stack **stack_a, t_stack **stack_b)
{
	t_stack	*curr_a;
	t_stack	*curr_b;
	t_stack	*target_node;

	curr_b = *stack_b;
	while (curr_b)
	{
		target_node = NULL;
		curr_a = *stack_a;
		while (curr_a)
		{
			if (curr_a->value > curr_b->value
				&& (!target_node || curr_a->value < target_node->value))
				target_node = curr_a;
			curr_a = curr_a->next;
		}
		if (!target_node)
			curr_b->target = find_min(*stack_a);
		else
			curr_b->target = target_node;
		curr_b = curr_b->next;
	}
}

void	calculate_cost_b(t_stack *stack_a, t_stack *stack_b)
{
	int		size_a;
	int		size_b;

	if (!stack_a || !stack_b)
		return ;
	size_a = stack_size(stack_a);
	size_b = stack_size(stack_b);
	while (stack_b)
	{
		if (stack_b->above_median)
			stack_b->cost_b = stack_b->pos;
		else
			stack_b->cost_b = size_b - stack_b->pos;
		if (stack_b->target->pos <= size_a / 2)
			stack_b->cost_b += stack_b->target->pos;
		else
			stack_b->cost_b += size_a - stack_b->target->pos;
		stack_b = stack_b->next;
	}
}

void	move_to_a(t_stack **stack_a, t_stack **stack_b)
{
	t_stack		*cheapest_node_b;

	if (!stack_b || !*stack_b)
		return ;
	cheapest_node_b = get_cheapest_b(*stack_b);
	if (!cheapest_node_b || !cheapest_node_b->target)
		return ;
	prep_for_push(stack_b, cheapest_node_b, 'b');
	prep_for_push(stack_a, cheapest_node_b->target, 'a');
	push_a(stack_b, stack_a);
}

t_stack	*get_cheapest_b(t_stack *stack_b)
{
	t_stack		*cheapest;

	if (!stack_b)
		return (NULL);
	cheapest = stack_b;
	while (stack_b)
	{
		if (stack_b->cost_b < cheapest->cost_b)
			cheapest = stack_b;
		stack_b = stack_b->next;
	}
	return (cheapest);
}
