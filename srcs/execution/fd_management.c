/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_management.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:09:10 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/17 16:20:01 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	save_standard_fds(t_shell *s)
{
	s->std_fds[0] = dup(STDIN_FILENO);
	s->std_fds[1] = dup(STDOUT_FILENO);
}

void	restore_standard_fds(t_shell *s)
{
	dup2(s->std_fds[0], STDIN_FILENO);
	dup2(s->std_fds[1], STDOUT_FILENO);
	close(s->std_fds[0]);
	close(s->std_fds[1]);
}

t_error	handle_heredoc_execution(t_token *redir, t_shell *s)
{
	int		fd;
	char	*content;
	t_error	res;

	res = SUCCESS;
	fd = create_heredoc_file();
	if (fd == -1)
		return (file_error("heredoc", s));
	content = redir->next->next->content;
	if (write(fd, content, ft_strlen(content)) == -1)
		res = file_error("heredoc", s);
	if (res == SUCCESS && content[ft_strlen(content)-1] != '\n')
		write(fd, "\n", 1);
	close(fd);
	fd = open("/tmp/minishell_heredoc", O_RDONLY);
	if (fd == -1)
		return (file_error("heredoc", s));
	if (dup2(fd, STDIN_FILENO) == -1)
		res = ERR_REDIR;
	close(fd);
	unlink("/tmp/minishell_heredoc");
	return (res);
}
