/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:39:28 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/14 16:58:55 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* t_error	execute_command(t_shell *s)
{
	t_token	*cmd_copy;
	t_error	res;

	cmd_copy = copy_tokens(s->token_list);
	if (!cmd_copy)
		return (ERR_MALLOC);
	if (has_pipe(s))
		res = handle_pipe_operations(s);
	else
		res = execute_single_command(cmd_copy, s);
	token_clear(&cmd_copy);
	return (res);
} */

t_error execute_command(t_shell *s)
{
	t_token	*cmd_copy;
	t_error	res;
	t_token	*temp;

	cmd_copy = copy_tokens(s->token_list);
	if (cmd_copy == NULL)
	    return (ERR_MALLOC);
	while (cmd_copy && cmd_copy->content && cmd_copy->content[0] == '\0')
	{
		temp = cmd_copy;
		cmd_copy = cmd_copy->next;
		free(temp->content);
		free(temp);
	}
	if (cmd_copy == NULL)
	{
		s->exit_status = 0;
		return (SUCCESS);
	}
	if (has_pipe(s))
		res = handle_pipe_operations(s);
	else
		res = execute_single_command(cmd_copy, s);
	token_clear(&cmd_copy);
	return (res);
}
