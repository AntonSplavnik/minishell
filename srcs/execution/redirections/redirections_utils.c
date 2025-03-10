/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:28:13 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/07 14:59:37 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	file_error(char *filename, t_shell *s)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	s->exit_status = 1;
	return (ERR_REDIR);
}

t_error	syntax_error(char *msg, t_shell *s)
{
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
	s->exit_status = 2;
	return (ERR_SYNTAX);
}

int	is_redirection(t_token_type type)
{
	return (type == TYPE_REDIRIN || type == TYPE_REDIROUT
			|| type == TYPE_REDIRAPPEND || type == TYPE_HEREDOC_OP);
}


t_token	*remove_redirections(t_token *cmd)
{
	t_token *clean;
	t_token *current;
	t_token *new;

	clean = NULL;
	current = cmd;
	while (current)
	{
		if (!is_redirection(current->type))
		{
			new = token_new(current->content, current->type);
			token_add_back(&clean, new);
		}
		else
			current = current->next;
		current = current->next;
	}
	return (clean);
}