/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:11 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/06 16:44:35 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	has_pipe
	handle_pipe_operations
	execute_pipeline
*/

/*
Purpose:
Checks whether the current command input contains pipes (|).

Functionality:
Calls count_pipes(s->token_list) to determine the number of pipes.
Returns 1 if there are pipes, otherwise returns 0.
*/
int	has_pipe(t_shell *s)
{
	return (count_pipes(s->token_list) > 0);
}


/*
Purpose:
Manages execution of commands connected via pipes.

Functionality:
Counts the number of pipes in the command input.
If there are no pipes, returns a parsing error.
Calls execute_pipeline(s, cmd_count) to execute the piped commands.
*/
t_error	handle_pipe_operations(t_shell *s)
{
	int		pipe_count;
	int		cmd_count;
	// t_error	result;

	pipe_count = count_pipes(s->token_list);
	if (pipe_count == 0)
		return (ERR_PARSING);
	cmd_count = pipe_count + 1;
	return (execute_pipeline(s, cmd_count));
}

/*
Purpose:
Executes multiple commands connected by pipes.

Functionality:
Initializes prev_pipe for tracking pipe file descriptors.
Iterates through commands, calling process_pipe_stage(s, &current, &prev_pipe, cmd_count) for each stage.
Waits for all child processes to finish execution before returning.
*/
t_error	execute_pipeline(t_shell *s, int cmd_count)
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