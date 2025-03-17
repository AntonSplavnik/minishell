/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:29:58 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/17 15:25:51 by asplavni         ###   ########.fr       */
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

// t_error	read_heredoc_input(int fd, char *delim)
// {
// 	char	*line;
// 	int		delim_len;

// 	delim_len = ft_strlen(delim);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strncmp(line, delim, delim_len + 1) == 0)
// 			break ;
// 		ft_putendl_fd(line, fd);
// 		free(line);
// 	}
// 	if (line)
// 		free(line);
// 	return (SUCCESS);
// }

t_error read_heredoc_input(int fd, char *delim)
{
    char    *line;
    size_t  n;
    int     delim_len;
    t_error res;
    int     line_len;

    delim_len = ft_strlen(delim);
    res = SUCCESS;
    n = 0;
    line = NULL;
    while (1)
    {
        write(1, "> ", 2);
        if (getline(&line, &n, stdin) == -1)
        {
            if (line)
            {
                free(line);
            }
            break;
        }
        line_len = ft_strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n')
        {
            line[line_len - 1] = '\0';
        }
        if (ft_strcmp(line, delim) == 0)
        {
            free(line);
            break;
        }
        ft_putendl_fd(line, fd);
        free(line);
        line = NULL;
        n = 0;
    }
    return (res);
}
