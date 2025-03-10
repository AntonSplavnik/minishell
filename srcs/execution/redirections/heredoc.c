/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:29:58 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/09 13:21:53 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_heredoc_file(void)
{
	int	fd;

	fd = open("/tmp/minishell_heredoc", O_CREAT|O_WRONLY|O_TRUNC, 0644);
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

t_error	read_heredoc_input(int fd, char *delim)
{
	char *line;
	int delim_len;

	delim_len = ft_strlen(delim);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delim, delim_len + 1) == 0)
			break;
		ft_putendl_fd(line, fd);
		free(line);
	}
	if (line)
		free(line);
	return (SUCCESS);
}

t_error	handle_heredoc_(t_token *redir, t_shell *s)
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
	{
		close(fd);
		return (file_error("heredoc", s));
	}
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