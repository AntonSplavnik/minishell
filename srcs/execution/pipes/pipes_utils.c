/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:24 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/18 12:45:53 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		setup_pipe_info(&pinfo, prev_pipe, pipe_fd, cmd_count);
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

static t_token	*copy_command_tokens(t_token *cmd_start, int token_count)
{
	t_token	*cmd_copy;
	t_token	*current;
	t_token	*new_token;
	int		i;

	i = 0;
	cmd_copy = NULL;
	current = cmd_start;
	while (i < token_count)
	{
		new_token = token_new(current->content, current->type);
		if (!new_token)
		{
			token_clear(&cmd_copy);
			return (NULL);
		}
		token_add_back(&cmd_copy, new_token);
		current = current->next;
		i++;
	}
	return (cmd_copy);
}

static void	find_command_range(t_token **tokens, t_token **start, \
								int *count)
{
	t_token	*current;

	*start = *tokens;
	while (*start && (*start)->content[0] == '\0')
		*start = (*start)->next;
	*count = 0;
	if (*start)
	{
		current = *start;
		while (current && ft_strcmp(current->content, "|") != 0)
		{
			(*count)++;
			current = current->next;
		}
	}
}

t_token	*get_next_cmd(t_token **tokens)
{
	t_token	*cmd_start;
	t_token	*current;
	t_token	*cmd_copy;
	int		token_count;

	find_command_range(tokens, &cmd_start, &token_count);
	if (!cmd_start)
	{
		*tokens = NULL;
		return (NULL);
	}
	cmd_copy = copy_command_tokens(cmd_start, token_count);
	current = cmd_start;
	while (token_count > 0)
	{
		current = current->next;
		token_count--;
	}
	if (current && ft_strcmp(current->content, "|") == 0)
		*tokens = current->next;
	else
		*tokens = NULL;
	return (cmd_copy);
}
