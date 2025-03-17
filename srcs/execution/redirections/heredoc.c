/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:29:58 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/17 16:19:51 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_heredoc_file(void)
{
	int	fd;

	fd = open("/tmp/minishell_heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	return (fd);
}

void	save_stdin(t_shell *s)
{
	s->stdin_copy = dup(STDIN_FILENO);
}

void	restore_stdin(t_shell *s)
{
	dup2(s->stdin_copy, STDIN_FILENO);
	close(s->stdin_copy);
}
