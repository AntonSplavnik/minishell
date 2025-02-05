/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_to_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:54:58 by abillote          #+#    #+#             */
/*   Updated: 2025/02/05 14:20:27 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//this file should be used for printf functions,
//helping us to debug our code - we should delete it before submitting

#include "../includes/minishell.h"

/*
Global array mapping token types to their string representations
Used for debugging to print human-readable token types
*/
const char	*g_token_type_str[] = {
[TYPE_ARG] = "TYPE_ARG",
[TYPE_COMMAND] = "TYPE_COMMAND",
[TYPE_DQUOTE] = "TYPE_DQUOTE",
[TYPE_SQUOTE] = "TYPE_SQUOTE",
[TYPE_PIPE] = "TYPE_PIPE",
[TYPE_REDIRIN] = "TYPE_REDIRIN",
[TYPE_REDIROUT] = "TYPE_REDIROUT",
[TYPE_REDIRAPPEND] = "TYPE_REDIAPPEND",
[TYPE_HEREDOC_OP] = "TYPE_HEREDOC_OP",
[TYPE_HEREDOC_DELIM] = "TYPE_HEREDOC_DELIM",
[TYPE_HEREDOC_CONTENT] = "TYPE_HEREDOC_CONTENT",
[TYPE_HEREDOC_CONTENT_QUOTED] = "TYPE_HEREDOC_CONTENT_QUOTED",
[TOKEN_EMPTY] = "TOKEN_EMPTY"
};

/*
Debug function to print token list contents:
- Prints each token's content and type
- Uses g_token_type_str for readable type names
- Shows total number of tokens
*/
void	print_token(t_token *token_list)
{
	t_token	*current;
	int		node_count;

	current = token_list;
	node_count = 0;
	while (current)
	{
		ft_printf("Node %d:\n", node_count++);
		ft_printf("  ├─ Content: %s\n", current->content);
		ft_printf("  └─ Type: %s\n", g_token_type_str[current->type]);
		current = current->next;
	}
	ft_printf("Total nodes: %d\n", node_count);
}

/*
Debug function to print environment list contents:
- Prints each variable's key and value
- Shows total number of environment variables
*/
void	print_env(t_env *env_list)
{
	t_env	*current;
	int		node_count;

	current = env_list;
	node_count = 0;
	while (current)
	{
		ft_printf("Variables %d:\n", node_count++);
		ft_printf("  ├─ Key: '%s'\n", current->key);
		ft_printf("  └─ Value: %s\n", current->value);
		current = current->next;
	}
	ft_printf("Total env variables: %d\n", node_count);
}
