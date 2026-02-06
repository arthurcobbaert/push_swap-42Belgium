/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acobbaer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:56:07 by acobbaer          #+#    #+#             */
/*   Updated: 2026/02/06 15:57:45 by acobbaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

bool	is_valid_input(char	**argv)
{
	int		i;
	int		j;
	long	num;
	long	tmp;

	i = 0;
	while (argv[i])
	{
		if (!ft_isdigit(argv[i]))
			return (false);
		num = ft_atol(argv[i]);
		if (num > INT_MAX || num < INT_MIN)
			return (false);
		j = 0;
		while (j < i)
		{
			tmp = ft_atol(argv[j]);
			if (num == tmp)
				return (false);
			j++;
		}
		i++;
	}
	return (true);
}
