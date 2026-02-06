/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acobbaer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:15:41 by acobbaer          #+#    #+#             */
/*   Updated: 2026/02/06 18:27:52 by acobbaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	free_argv(char **splitted_argv)
{
	int	i;

	i = 0;
	if (!splitted_argv)
		return ;
	while (splitted_argv[i])
	{
		free(splitted_argv[i]);
		i++;
	}
	free(splitted_argv);
}

void	free_stack(t_stack **stack)
{
	t_stack		*tmp;

	while (*stack)
	{
		tmp = (*stack)->next;
		free(*stack);
		*stack = tmp;
	}
}
