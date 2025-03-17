/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:56:54 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/17 15:27:18 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	execute_single_command(t_token *cmd, t_shell *s)
{
	t_token	*clean_cmd;
	char	**args;
	t_error	res;

	save_standard_fds(s);
	res = handle_redirections_(cmd, s);
	if (res != SUCCESS)
		return (restore_standard_fds(s), res);
	clean_cmd = remove_redirections(cmd);
	args = prepare_command_args(clean_cmd);
	if (!args)
		return (restore_standard_fds(s), handle_malloc_error(s));
	if (is_builtin(clean_cmd->content))
	{
		res = execute_builtin(clean_cmd, args, s);
		restore_standard_fds(s);
	}
	else
		res = execute_external_command(clean_cmd, args, s);
	token_clear(&clean_cmd);
	restore_standard_fds(s);
	free_array(args);
	return (res);
}

t_error	handle_builtin_redirections(t_token *cmd, t_shell *s)
{
	t_error	res;

	res = handle_redirections_(cmd, s);
	if (res != SUCCESS)
		restore_standard_fds(s);
	return (res);
}

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
