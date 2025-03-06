/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:24 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/06 16:53:04 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	process_parent
	process_pipe_stage
	count_pipes
	get_next_cmd
*/

t_error process_parent(int *prev_pipe, int pipe_fd[2], int cmd_count)
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

/*
Purpose:
Handles execution of a single stage in a pipeline.

Functionality:
Extracts the next command from current tokens.
Creates a pipe and forks a child process.
Calls process_child for the child process or process_parent for the parent process.
*/
t_error	process_pipe_stage(t_shell *s, t_token **current,
									int *prev_pipe, int cmd_count)
{
int	pipe_fd[2];
pid_t   pid;
t_token *cmd;
t_error result;

cmd = get_next_cmd(current);
result = create_pipe_and_fork(cmd_count, pipe_fd, &pid);
if (result != SUCCESS)
return (result);
if (pid == 0)
return (process_child(cmd, *prev_pipe, cmd_count, pipe_fd, s));
return (process_parent(prev_pipe, pipe_fd, cmd_count));
}

/*
Purpose:
Counts the number of pipes (|) in the given token list.

Functionality:
Iterates through tokens, incrementing a counter for each | found.
Returns the total count of pipes.
*/
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

/*
Purpose:
Extracts the next command in the pipeline from the token list.

Functionality:
Traverses tokens until it finds a pipe (|).
Splits the token list at the pipe and returns the command segment before it.
*/
t_token	*get_next_cmd(t_token **tokens)
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
