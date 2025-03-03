/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:39:28 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/03 18:19:24 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	execute_command
	execute_single_command
	execute_external_command
*/

t_error	execute_command(t_token *cmd, t_shell *s)
{
	t_token *cmd_copy;
	t_error res;

	cmd_copy = copy_tokens(cmd);
	if (!cmd_copy)
		return (ERR_MALLOC);
	if (has_pipe(cmd))
		res = handle_pipe_operations(s);
	else
		res = execute_single_command(cmd_copy, s);
	token_clear(&cmd_copy);
	return (res);
}

t_error	execute_single_command(t_token *cmd, t_shell *s)
{
	char	**args;
	t_error	res;

	args = prepare_command_args(cmd);
	if (!args)
		return (handle_malloc_error(s));
	if (has_redirection(cmd))
		res = handle_redirections(cmd, s);
	if (res == SUCCESS)
	{
		if (is_builtin(cmd->content))
			res = execute_builtin(cmd, args, s);
		else
			res = execute_external_command(cmd, args, s);
	}
	free_array(args);
	return (res);
}

t_error	execute_external_command(t_token *cmd, char **args, t_shell *s)
{
	char	*cmd_path;
	t_error result;

	cmd_path = find_command_path(cmd->content, s);
	if (!cmd_path)
		return (ERR_CMD_NOT_FOUND);
	result = execute_child_process(cmd_path, args, s);
	return (result);
}