/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:32:50 by asplavni          #+#    #+#             */
/*   Updated: 2025/02/28 17:18:56 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int has_pipe(t_shell *s)
{
	return (count_pipes(s->token_list) > 0);
}

static int count_pipes(t_token *tokens)
{
	t_token *current;
	int count;

	current = tokens;
	count = 0;
	while (current)
	{
		if (ft_strcmp(current->content, "|") == 0)
			count++;
		current = current->next;
	}
	return (count);
}

static t_error	process_child(t_token *cmd, int prev_pipe, int cmd_count,
	int pipe_fd[2], t_shell *s)
{
	int out_fd;

	set_output_fd(cmd_count, pipe_fd, &out_fd);
	handle_child_process_io(prev_pipe, out_fd); //how?
	if (prev_pipe != -1)//how? what?
		close(prev_pipe);//how? what?
	if (cmd_count > 0)//how? what?
		close(pipe_fd[0]);//how? what?
	execute_single_command(cmd, s);
	exit(s->exit_status);
	return (SUCCESS);
}

static t_error	process_parent(int *prev_pipe, int pipe_fd[2], int cmd_count)
{
	if (*prev_pipe != -1)
		close(*prev_pipe);
	if (cmd_count > 0)
	{
		*prev_pipe = pipe_fd[0];
		close(pipe_fd[1]);
	}
	return (SUCCESS);
}

static t_error process_pipe_stage(t_shell *s, t_token **current,
	int *prev_pipe, int cmd_count)
{
	int		pipe_fd[2];
	pid_t	pid;
	t_token	*cmd;
	t_error	result;

	cmd = get_next_cmd(current);
	result = create_pipe_and_fork(cmd_count, pipe_fd, &pid);
	if (result != SUCCESS)
		return (result);
	if (pid == 0)
		return (process_child(cmd, *prev_pipe, cmd_count, pipe_fd, s));
	else
		return (process_parent(prev_pipe, pipe_fd, cmd_count));
}


static t_error execute_pipeline(t_shell *s, int cmd_count)
{
	int		prev_pipe;
	t_token	*current;
	t_error	result;

	prev_pipe = -1;
	current = s->token_list;
	result = SUCCESS;
	while (cmd_count-- > 0 && result == SUCCESS)
		result = process_pipe_stage(s, &current, &prev_pipe, cmd_count);
	while (waitpid(-1, NULL, 0) > 0)
		;
	return (result);
}


t_error handle_pipe_operations(t_shell *s)
{
	int	pipe_count;

	pipe_count = count_pipes(s->token_list);
	if (pipe_count == 0)
		return (ERR_PARSING);
	return (execute_pipeline(s, pipe_count + 1));
}
