/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:22:24 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/13 14:33:06 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir_type	get_redir_type(t_token_type type)
{
	t_redir_type	rtype;

	rtype = REDIR_OUT;
	if (type == TYPE_REDIROUT)
		return (REDIR_OUT);
	if (type == TYPE_REDIRAPPEND)
		return (REDIR_APPEND);
	if (type == TYPE_REDIRIN)
		return (REDIR_IN);
	return (REDIR_HEREDOC);
}

t_error	handle_output(t_token *redir, t_shell *s)
{
	int		fd;
	char	*filename;
	int		flags;

	filename = redir->next->content;
	flags = O_WRONLY | O_CREAT;
	if (get_redir_type(redir->type) == REDIR_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
		return (file_error(filename, s));
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (ERR_REDIR);
	close(fd);
	return (SUCCESS);
}

t_error	handle_input(t_token *redir, t_shell *s)
{
	int		fd;
	char	*filename;

	filename = redir->next->content;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (file_error(filename, s));
	if (dup2(fd, STDIN_FILENO) == -1)
		return (ERR_REDIR);
	close(fd);
	return (SUCCESS);
}

t_error	handle_redirections_(t_token *cmd, t_shell *s)
{
	t_token	*current;
	t_error	res;

	current = cmd;
	res = SUCCESS;
	while (current && res == SUCCESS)
	{
		if (is_redirection(current->type))
		{
			if (!current->next || current->next->type == TYPE_PIPE)
				return (syntax_error("near unexpected token", s));
			if (get_redir_type(current->type) == REDIR_HEREDOC)
				res = handle_heredoc_(current, s);
			else if (get_redir_type(current->type) == REDIR_IN)
				res = handle_input(current, s);
			else
				res = handle_output(current, s);
			current = current->next->next;
		}
		else
			current = current->next;
	}
	return (res);
}
