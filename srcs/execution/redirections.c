/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:10:08 by asplavni          #+#    #+#             */
/*   Updated: 2025/02/28 13:23:27 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_redirection(t_shell *s)
{
	t_token	*current;

	current = s->token_list;
	while (current)
	{
		if (is_redirection_token(current))
			return (1);
		current = current->next;
	}
	return (0);
}

t_error	execute_redirection(t_shell *s)
{
	t_token	*current;
	t_error	result;

	current = s->token_list;
	result = SUCCESS;
	while (current)
	{
		if (is_redirection_token(current))
			result = handle_redirection_type(current, s);
		current = current->next;
	}
	return (result);
}

static int	is_redirection_token(t_token *token)
{
	return (ft_strcmp(token->content, "<") == 0
		|| ft_strcmp(token->content, ">") == 0
		|| ft_strcmp(token->content, "<<") == 0
		|| ft_strcmp(token->content, ">>") == 0);
}

static t_error	handle_redirection_type(t_token *token, t_shell *s)
{
	if (ft_strcmp(token->content, "<") == 0)
		return (handle_input_redir(s));
	if (ft_strcmp(token->content, ">") == 0)
		return (handle_output_redir(s));
	if (ft_strcmp(token->content, "<<") == 0)
		return (handle_heredoc(s));
	if (ft_strcmp(token->content, ">>") == 0)
		return (handle_append_redir(s));
	return (SUCCESS);
}


t_error	handle_input_redir(t_shell *s)
{
	// Input redirection implementation
}

t_error	handle_output_redir(t_shell *s)
{
	// Output redirection implementation
}

t_error	handle_append_redir(t_shell *s)
{
	// Append redirection implementation
}

t_error	handle_heredoc(t_shell *s)
{
	// Heredoc implementation
}