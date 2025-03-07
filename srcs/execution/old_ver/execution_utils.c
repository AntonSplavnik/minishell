/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 07:59:21 by abillote          #+#    #+#             */
/*   Updated: 2025/02/28 13:48:17 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Return the number of args needed for the command execution
*/
int	count_command_args(t_token *cmd_token)
{
	t_token	*current;
	int		arg_count;

	arg_count = 1;
	current = cmd_token->next;
	while (current && current->type != TYPE_PIPE)
	{
		if (current->type == TYPE_ARG)
			arg_count++;
		current = current->next;
	}
	return (arg_count);
}

/*
Prepares command arguments array from token list
Returns array of strings suitable for execve:
- args[0] should be the file name
*/
char	**prepare_command_args(t_token *cmd_token)
{
	t_token	*current;
	char	**args;
	int		arg_count;
	int		i;

	arg_count = count_command_args(cmd_token);
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	args[0] = ft_strdup(cmd_token->content);
	i = 1;
	current = cmd_token->next;
	while (current && current->type != TYPE_PIPE)
	{
		if (current->type == TYPE_ARG)
		{
			args[i] = ft_strdup(current->content);
			i++;
		}
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}
