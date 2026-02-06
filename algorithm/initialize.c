/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acobbaer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:10:33 by acobbaer          #+#    #+#             */
/*   Updated: 2026/02/06 19:49:07 by acobbaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_stack	*init_stack_a(char **args)
{
	int		i;
	long	num;
	t_stack	*stack_a;
	t_stack	*new_node;

	stack_a = NULL;
	i = 0;
	while (args[i])
	{
		num = ft_atol(args[i]);
		new_node = add_node((int)num);
		if (!new_node)
			return (free_stack(&stack_a), NULL);
		stack_add_bottom(&stack_a, new_node);
		i++;
	}
	return (stack_a);
}

t_stack	*add_node(int num)
{
	t_stack		*node;

	node = malloc(sizeof * node);
	if (!node)
		return (NULL);
	node->value = num;
	node->index = 0;
	node->pos = -1;
	node->cost_a = -1;
	node->cost_b = -1;
	node->target = NULL;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	stack_add_bottom(t_stack **stack_a, t_stack *new)
{
	t_stack		*last_node;

	if (!new)
		return ;
	if (!*stack_a)
	{
		*stack_a = new;
		return ;
	}
	last_node = get_last_node(*stack_a);
	new->prev = last_node;
	new->next = NULL;
	last_node->next = new;
}

t_stack	*get_last_node(t_stack *stack_a)
{
	while (stack_a && stack_a->next != NULL)
		stack_a = stack_a->next;
	return (stack_a);
}
