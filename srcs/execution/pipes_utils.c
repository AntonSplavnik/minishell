/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:06:18 by asplavni          #+#    #+#             */
/*   Updated: 2025/02/28 16:46:57 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void set_output_fd(int cmd_count, int pipe_fd[2], int *out_fd)
{
	if (cmd_count > 0)
		*out_fd = pipe_fd[1];
	else
		*out_fd = STDOUT_FILENO;
}

static void handle_child_process_io(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

static void set_output_fd(int cmd_count, int pipe_fd[2], int *out_fd)
{
	if (cmd_count > 0)
		*out_fd = pipe_fd[1];
	else
		*out_fd = STDOUT_FILENO;
}

static t_error create_pipe_and_fork(int cmd_count, int pipe_fd[2], pid_t *pid)
{
	if (cmd_count > 0 && pipe(pipe_fd) == -1)
		return (ERR_EXEC);
	*pid = fork();
	if (*pid == -1)
		return (ERR_EXEC);
	return (SUCCESS);
}


static t_token *get_next_cmd(t_token **tokens)
{
	t_token *cmd_start;
	t_token *current;

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