/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acobbaer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 18:08:45 by acobbaer          #+#    #+#             */
/*   Updated: 2026/02/06 18:32:52 by acobbaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stddef.h>
# include <stdio.h> 
# include <stdbool.h>

//Struct

typedef struct s_stack
{
	int					value;
	int					index;
	int					pos;
	int					cost_a;
	int					cost_b;
	struct s_stack		*target;
	struct s_stack		*prev;
	struct s_stack		*next;
	bool			above_median;
}			t_stack;

//Parse arguments

bool	is_valid_input(char **argv);
bool	ft_isdigit(char *nb);

//Utils

char	**ft_split(char *str, char c);
void	ft_putstr(char *str);
long	ft_atol(const char *str);
int		stack_size(t_stack *stack);
bool	is_sorted(t_stack **stack);

//Initialize stack

t_stack	*init_stack_a(char **args);
t_stack	*add_node(int num);
void	stack_add_bottom(t_stack **stack_a, t_stack *new);
t_stack	*get_last_node(t_stack *stack_a);
void	init_nodes_a(t_stack **stack_a, t_stack **stack_b);

//Operations

void	push_b(t_stack **stack_a, t_stack **stack_b);
void	push_a(t_stack **stack_b, t_stack **stack_a);
void	reverse_rotate_a(t_stack **stack_a);
void	reverse_rotate_b(t_stack **stack_b);
void	reverse_rotate_both(t_stack **stack_a, t_stack **stack_b);
void	rotate_a(t_stack **stack_a);
void	rotate_b(t_stack **stack_b);
void	rotate_both(t_stack **stack_a, t_stack **stack_b);
void	swap_a(t_stack **stack_a);
void	swap_b(t_stack **stack_b);
void	swap_both(t_stack **stack_a, t_stack **stack_b);

//Sorting

void	push_swap(t_stack **stack_a, t_stack **stack_b);
void	sort_tiny(t_stack **stack_a);
t_stack	*find_max(t_stack *stack);
t_stack	*find_min(t_stack *stack);
void	sort_stacks(t_stack **stack_a, t_stack **stack_b);
void	find_target_node(t_stack **stack_a, t_stack **stack_b);
void	calculate_cost(t_stack *stack_a, t_stack *stack_b);
void	move_to_b(t_stack **stack_a, t_stack **stack_b);
void	prep_for_push(t_stack **stack, t_stack *node_to_top, char stack_name);
t_stack	*get_cheapest(t_stack *stack_a);
void	init_nodes_b(t_stack **stack_a, t_stack **stack_b);
void	find_target_node_b(t_stack **stack_a, t_stack **stack_b);
void	calculate_cost_b(t_stack *stack_a, t_stack *stack_b);
void	move_to_a(t_stack **stack_a, t_stack **stack_b);
t_stack	*get_cheapest_b(t_stack *stack_b);
void	min_on_top(t_stack **stack_a);
void	sort_five(t_stack **stack_a, t_stack **stack_b);

//Free memory

void	free_stack(t_stack **stack);
void	free_argv(char **splitted_argv);

//Assign values to each node

void	assign_pos(t_stack *stack);
void	assign_index(t_stack *stack);

#endif
