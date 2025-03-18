/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:24 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/18 10:20:08 by abillote         ###   ########.fr       */
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
	// printf("in process pipe stage, after get next cmd, token list is\n");
	// print_token(s->token_list);
	// printf("\n");
	result = create_pipe_and_fork(cmd_count, pipe_fd, &pid);
	if (result != SUCCESS)
		return (result);
	if (pid == 0)
	{
		pinfo.prev_pipe = *prev_pipe;
		pinfo.cmd_count = cmd_count;
		pinfo.pipe_fd[0] = pipe_fd[0];
		pinfo.pipe_fd[1] = pipe_fd[1];
		result = process_child(cmd, &pinfo, s);
		token_clear(&cmd);
		exit(s->exit_status);
	}
	else
	{
		if (cmd_count == 0)
			s->last_pid = pid;
		token_clear(&cmd);
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
	t_token *cmd_start;
	t_token *current;
	t_token *cmd_copy = NULL;
	int token_count = 0;

	// Find the first command (skipping empty tokens)
	cmd_start = *tokens;
	while (cmd_start && cmd_start->content[0] == '\0')
		cmd_start = cmd_start->next;

	// If no valid command, return NULL
	if (!cmd_start)
	{
		*tokens = NULL;
		return NULL;
	}

	// Count tokens until pipe or end of list
	current = cmd_start;
	while (current && ft_strcmp(current->content, "|") != 0)
	{
		token_count++;
		current = current->next;
	}

	// Copy only the command tokens (not including pipe)
	current = cmd_start;
	for (int i = 0; i < token_count; i++)
	{
		t_token *new_token = token_new(current->content, current->type);
		if (!new_token)
		{
			token_clear(&cmd_copy);
			return NULL;
		}
		token_add_back(&cmd_copy, new_token);
		current = current->next;
	}

	// Update tokens pointer to point after the pipe
	if (current && ft_strcmp(current->content, "|") == 0)
		*tokens = current->next;  // Skip the pipe token
	else
		*tokens = NULL;  // No more tokens
	// printf("in get next cmd, after detaching pipe list is\n");
	// print_token(*tokens);
	// printf("\n");
	return cmd_copy;
}
