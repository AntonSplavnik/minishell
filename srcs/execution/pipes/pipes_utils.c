/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:24 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/17 10:07:49 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	process_parent(int *prev_pipe, int pipe_fd[2], int cmd_count)
{
	if (*prev_pipe != -1)
	{
		close(*prev_pipe);
		*prev_pipe = -1;
	}
	if (cmd_count > 0)
	{
		*prev_pipe = pipe_fd[0];
		close(pipe_fd[1]);
	}
	return (SUCCESS);
}

t_error	process_pipe_stage(t_shell *s, t_token **current, int *prev_pipe,
															int cmd_count)
{
	int			pipe_fd[2];
	pid_t		pid;
	t_token		*cmd;
	t_error		result;
	t_pipe_info	pinfo;

	cmd = get_next_cmd(current);
	result = create_pipe_and_fork(cmd_count, pipe_fd, &pid);
	if (result != SUCCESS)
		return (result);
	if (pid == 0)
	{
		pinfo.prev_pipe = *prev_pipe;
		pinfo.cmd_count = cmd_count;
		pinfo.pipe_fd[0] = pipe_fd[0];
		pinfo.pipe_fd[1] = pipe_fd[1];
		return (process_child(cmd, &pinfo, s));
	}
	else
	{
		if (cmd_count == 0)
			s->last_pid = pid;
		return (process_parent(prev_pipe, pipe_fd, cmd_count));
	}
}

int	count_pipes(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (ft_strcmp(tokens->content, "|") == 0)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

t_token	*get_next_cmd(t_token **tokens)
{
	t_token	*cmd_start;
	t_token	*current;

	cmd_start = *tokens;
	current = *tokens;
	while (current && ft_strcmp(current->content, "|") != 0)
		current = current->next;
	if (current)
	{
		*tokens = current->next;
		current->next = NULL;
	}
	else
		*tokens = NULL;
	return (cmd_start);
}
