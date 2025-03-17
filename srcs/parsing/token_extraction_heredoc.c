/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:27:25 by abillote          #+#    #+#             */
/*   Updated: 2025/03/17 15:04:30 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>

/*
** Initializes heredoc content collection process
** Returns: SUCCESS, ERR_MALLOC, or ERR_EXEC
*/
t_error	init_heredoc_collection(char *delimiter, char **content_ptr, \
									int *stdin_copy)
{
	*stdin_copy = dup(STDIN_FILENO);
	if (*stdin_copy == -1)
		return (ERR_EXEC);
	if (!delimiter || !content_ptr)
	{
		close(*stdin_copy);
		return (ERR_MALLOC);
	}
	*content_ptr = ft_strdup("");
	if (!*content_ptr)
	{
		close(*stdin_copy);
		return (ERR_MALLOC);
	}
	if (set_signals_heredoc())
	{
		free(*content_ptr);
		*content_ptr = NULL;
		close(*stdin_copy);
		return (ERR_SIGNAL);
	}
	g_sig = 0;
	return (SUCCESS);
}

/*
** Clean up resources when exiting heredoc collection
*/
t_error	cleanup_heredoc_collection(int stdin_copy, t_error return_code)
{
	close(stdin_copy);
	set_signals_interactive();
	return (return_code);
}

/*
** Main heredoc content collection loop
** Returns SUCCESS on normal completion or error code on failure
*/
t_error	collect_heredoc_content(char *delimiter,
								char **content_ptr)
{
	char	*line;
	int		stdin_copy;
	t_error	result;

	result = init_heredoc_collection(delimiter, content_ptr, &stdin_copy);
	if (result != SUCCESS)
		return (result);
	while (1)
	{
		line = readline("> ");
		result = process_heredoc_line(line, delimiter, content_ptr);
		if (result == ERR_SIGNAL)
			return (handle_heredoc_interruption(content_ptr, stdin_copy));
		if (result == ERR_DELIMITER)
			return (cleanup_heredoc_collection(stdin_copy, SUCCESS));
		if (result != SUCCESS)
			return (cleanup_heredoc_collection(stdin_copy, ERR_MALLOC));
	}
}

/*
** Adds a line to existing heredoc content
** Returns new concatenated content or NULL on error
*/
char	*handle_line_input(char *content, char *line)
{
	char	*temp;
	char	*new_content;

	if (!content || content[0] == '\0')
	{
		new_content = ft_strdup(line);
		free(line);
		return (new_content);
	}
	temp = ft_strjoin(content, "\n");
	free(content);
	if (!temp)
	{
		free(line);
		return (NULL);
	}
	new_content = ft_strjoin(temp, line);
	free(temp);
	free(line);
	return (new_content);
}

/*
** Determines token type based on heredoc info
** Returns appropriate token type
*/
t_token_type	get_heredoc_content_type(t_heredoc_info *heredoc_info)
{
	if (heredoc_info->is_quoted)
		return (TYPE_HEREDOC_CONTENT_QUOTED);
	return (TYPE_HEREDOC_CONTENT);
}
