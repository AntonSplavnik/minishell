/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:39:28 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/17 18:05:16 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	execute_command(t_shell *s)
{
	t_token	*cmd_copy;
	t_error	res;
	t_token	*temp;

	cmd_copy = s->token_list;
	while (cmd_copy && cmd_copy->content && cmd_copy->content[0] == '\0')
	{
		temp = cmd_copy;
		s->token_list = cmd_copy->next;
		cmd_copy = cmd_copy->next;
		free(temp->content);
		free(temp);
	}
	if (s->token_list == NULL)
	{
		s->exit_status = 0;
		return (SUCCESS);
	}
	if (has_pipe(s))
		res = handle_pipe_operations(s);
	else
	{
		cmd_copy = copy_tokens(s->token_list);
		if (!cmd_copy)
			return (ERR_MALLOC);
		res = execute_single_command(cmd_copy, s);
		token_clear(&cmd_copy);
	}
	return (res);
}
