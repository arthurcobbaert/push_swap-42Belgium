/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acobbaer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:04:32 by acobbaer          #+#    #+#             */
/*   Updated: 2026/02/06 16:05:13 by acobbaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}

long	ft_atol(const char *str)
{
	int		i;
	int		signal;
	long	result;

	i = 0;
	result = 0;
	signal = 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			signal = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (signal * result);
}

bool	ft_isdigit(char *nb)
{
	int	i;

	i = 0;
	if (nb[i] == '+' || nb[i] == '-')
		i++;
	if (!nb[i])
		return (false);
	while (nb[i])
	{
		if (nb[i] < '0' || nb[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	stack_size(t_stack *stack)
{
	int	i;

	i = 0;
	while (stack != NULL)
	{
		i++;
		stack = stack->next;
	}
	return (i);
}

bool	is_sorted(t_stack **stack)
{
	t_stack		*node;

	node = *stack;
	while (node->next != NULL)
	{
		if (node->value > node->next->value)
			return (false);
		node = node->next;
	}
	return (true);
}
