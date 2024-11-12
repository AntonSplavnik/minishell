/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_to_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:54:58 by abillote          #+#    #+#             */
/*   Updated: 2024/11/12 13:36:57 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//this file should be used for printf functions,
//helping us to debug our code - we should delete it before submitting

#include "../includes/minishell.h"

//this needs to be updated with all the types we define
const char	*g_token_type_str[] = {
[TYPE_WORD] = "TYPE_WORD",
[TYPE_COMMAND] = "TYPE_COMMAND",
[TYPE_QUOTES] = "TYPE_QUOTES",
[TOKEN_ERROR] = "TOKEN_ERROR"
};

void	print_token(t_token *token_list)
{
	t_token	*current;
	int		node_count;

	current = token_list;
	node_count = 0;
	while (current)
	{
		ft_printf("Node %d:\n", node_count++);
		ft_printf("  ├─ Content: '%s'\n", current->content);
		ft_printf("  └─ Type: %s\n", g_token_type_str[current->type]);
		current = current->next;
	}
	ft_printf("Total nodes: %d\n", node_count);
}
