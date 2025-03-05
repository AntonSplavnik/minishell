/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:56:54 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/05 18:02:53 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	execute_single_command
	execute_external_command
*/

/*
Purpose:
Executes a single command (either a built-in command or an external program).

Functionality:
Converts the command token list into an argument array.
If the command has redirections, handle_redirections(cmd, s) is called.
Determines if the command is a built-in:
If so, it executes it using execute_builtin(cmd, args, s).
Otherwise, execute_external_command(cmd, args, s) is called to run an external program.
Frees the allocated argument array before returning.
*/
t_error	execute_single_command(t_token *cmd, t_shell *s)
{
	char	**args;
	t_error	res;

	res = SUCCESS;
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

/*
Purpose:
Executes an external command by locating its executable path and spawning a child process.

Functionality:
Finds the full path of the command using find_command_path(cmd->content, s).
If the path is found, it launches the command in a child process using execute_child_process(cmd_path, args, s).
Returns an error if the command is not found.
*/
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
