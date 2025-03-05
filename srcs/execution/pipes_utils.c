/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:24 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/05 16:58:26 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	count_pipes
	set_output_fd
	get_next_cmd
	create_pipe_and_fork
*/

/*
Purpose:
Counts the number of pipes (|) in the given token list.

Functionality:
Iterates through tokens, incrementing a counter for each | found.
Returns the total count of pipes.
*/
static int count_pipes(t_token *tokens)
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

/*
Purpose:
Sets the output file descriptor based on whether the command is part of a pipeline.

Functionality:
If cmd_count > 0, assigns pipe_fd[1] (write end of the pipe) to out_fd.
Otherwise, sets out_fd to STDOUT_FILENO.
*/
static void set_output_fd(int cmd_count, int pipe_fd[2], int *out_fd)
{
	if (cmd_count > 0)
		*out_fd = pipe_fd[1];
	else
		*out_fd = STDOUT_FILENO;
}


/*
Purpose:
Extracts the next command in the pipeline from the token list.

Functionality:
Traverses tokens until it finds a pipe (|).
Splits the token list at the pipe and returns the command segment before it.
*/
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

/*
Purpose:
Creates a pipe and forks a new child process if necessary.

Functionality:
If cmd_count > 0, creates a pipe using pipe(pipe_fd).
Forks a new process using fork().
Returns ERR_EXEC if either operation fails.
*/
static t_error create_pipe_and_fork(int cmd_count, int pipe_fd[2], pid_t *pid)
{
	if (cmd_count > 0 && pipe(pipe_fd) == -1)
		return (ERR_EXEC);
	*pid = fork();
	if (*pid == -1)
		return (ERR_EXEC);
	return (SUCCESS);
}

/*
Purpose:
Executes a command as a child process within a pipeline.

Functionality:
Redirects input/output based on the pipeline structure.
Closes unnecessary file descriptors.
Calls execute_single_command(cmd, s) to execute the command.
Exits with the shell’s exit status.
*/
static t_error process_child(t_token *cmd, int prev_pipe, int cmd_count,
										int pipe_fd[2], t_shell *s)
{
	int	out_fd;

	set_output_fd(cmd_count, pipe_fd, &out_fd);
	handle_child_process_io(prev_pipe, out_fd);
	if (prev_pipe != -1)
		close(prev_pipe);
	if (cmd_count > 0)
		close(pipe_fd[0]);
	execute_single_command(cmd, s);
	exit(s->exit_status);
	return (SUCCESS);
}