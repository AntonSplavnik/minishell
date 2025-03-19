/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:11 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/19 13:42:28 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_pipe(t_shell *s)
{
	return (count_pipes(s->token_list) > 0);
}

t_error	handle_pipe_operations(t_shell *s)
{
	int		pipe_count;
	int		cmd_count;

	pipe_count = count_pipes(s->token_list);
	if (pipe_count == 0)
		return (ERR_PARSING);
	cmd_count = pipe_count + 1;
	return (execute_pipeline(s, cmd_count));
}

t_error	execute_pipeline(t_shell *s, int cmd_count)
{
	int		prev_pipe;
	int		status;
	t_token	*current;
	t_error	result;
	pid_t	pid;

	prev_pipe = -1;
	current = s->token_list;
	result = SUCCESS;
	while (cmd_count > 0 && result == SUCCESS)
	{
		cmd_count = cmd_count - 1;
		result = process_pipe_stage(s, &current, &prev_pipe, cmd_count);
	}
	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == s->last_pid)
			s->exit_status = WEXITSTATUS(status);
		pid = wait(&status);
	}
	return (result);
}
