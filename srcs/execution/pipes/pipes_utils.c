/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:40:24 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/18 15:49:34 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	find_command_range(t_token **tokens, t_token **start,
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
