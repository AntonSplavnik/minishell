/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:39:28 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/11 17:35:22 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	execute_command
*/

/*
When you process commands, especially with pipes, the original token list
gets modified.
If we don't copy tokens, each command in a pipeline might alter the original
list, leading to incorrect parsing for subsequent commands.
For example, if there's a pipe, the token list is split into segments.
Without copying, the original list would lose parts after processing the first
command, making it impossible to handle the next commands correctly.

Preserve Original Token List Integrity
	Original: [ls][-l][|][grep][test]
	After pipe handling: [ls][-l] (rest lost)

Prevent Concurrent Modification
	process1: Reading tokens
	process2: Modifying same token list (e.g., redirections)

 Enable Error Recovery
	Start processing cmd1 | cmd2
	cmd1 fails parsing
	Need to continue with cmd2

Support Recursive/Nested Processing
	echo $(ls | grep .c)

	Main tokens: [echo][$(...)]
	Nested copy: [ls][|][grep][.c]

Memory Safety
	free(original_tokens);

	Parent: keeps original
	Child: works on copy

Redirection Handling
	1. Copy tokens for command
	2. Process redirections in copy
	3. Remove redirection tokens from copy
	4. Execute core command
*/

/*
Purpose:
Executes a given command by determining whether it contains pipes and
dispatching it accordingly.

Functionality:
Makes a copy of the command tokens.
Checks if the command contains a pipe (|).
If it does, handle_pipe_operations(s) is called to process the pipeline.
Otherwise, the command is executed as a single command using
execute_single_command(cmd_copy, s).
Frees the copied token list before returning the result.
*/
t_error	execute_command(t_shell *s)
{
	t_token	*cmd_copy;
	t_error	res;

	cmd_copy = copy_tokens(s->token_list);
	if (!cmd_copy)
		return (ERR_MALLOC);
	if (has_pipe(s))
		res = handle_pipe_operations(s);
	else
		res = execute_single_command(cmd_copy, s);
	token_clear(&cmd_copy);
	return (res);
}
