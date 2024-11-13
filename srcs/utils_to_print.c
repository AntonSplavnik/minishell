/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_to_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:54:58 by abillote          #+#    #+#             */
/*   Updated: 2024/11/13 15:28:56 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//this file should be used for printf functions,
//helping us to debug our code - we should delete it before submitting

#include "../includes/minishell.h"

//this needs to be updated with all the types we define
const char	*g_token_type_str[] = {
[TYPE_ARG] = "TYPE_ARG",
[TYPE_COMMAND] = "TYPE_COMMAND",
[TYPE_DQUOTE] = "TYPE_DQUOTE",
[TYPE_SQUOTE] = "TYPE_SQUOTE",
[TYPE_PIPE] = "TYPE_PIPE",
[TYPE_REDIRIN] = "TYPE_REDIRIN",
[TYPE_REDIROUT] = "TYPE_REDIROUT",
[TYPE_REDIRAPPEND] = "TYPE_REDIAPPEND",
[TYPE_HEREDOC] = "TYPE_HEREDOC",
[TOKEN_EMPTY] = "TOKEN_EMPTY"
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
