/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:56:54 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/10 17:53:00 by asplavni         ###   ########.fr       */
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
	t_token	*clean_cmd;
	char	**args;
	t_error	res;
	int		std_fds[2];

	save_standard_fds(std_fds);
	res = handle_redirections_(cmd, s);
	if (res != SUCCESS)
		return (restore_standard_fds(std_fds), res);

	clean_cmd = remove_redirections(cmd);
	args = prepare_command_args(clean_cmd);
	if (!args)
		return (restore_standard_fds(std_fds), handle_malloc_error(s));
	if (is_built_in(clean_cmd->content))
	{
		res = execute_built_in(clean_cmd, args, s);
		restore_standard_fds(std_fds);
	}
	else
		res = execute_external_command(clean_cmd, args, s);
	token_clear(&clean_cmd);
	restore_standard_fds(std_fds);
	free_array(args);
	return (res);
}

t_error handle_builtin_redirections(t_token *cmd, t_shell *s, int std_fds[2])
{
    t_error res;

    res = handle_redirections_(cmd, s);
    if (res != SUCCESS)
        restore_standard_fds(std_fds);
    return res;
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
	t_error	res;

	cmd_path = find_command_path(cmd->content, s);
	if (!cmd_path)
		return (ERR_CMD_NOT_FOUND);
	res = execute_child_process(cmd_path, args, s);
	free(cmd_path);
	return (res);
}
