/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:27:25 by abillote          #+#    #+#             */
/*   Updated: 2025/01/15 16:05:33 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>

/*
Processes a single line of heredoc input:
- Takes current content and new line as input
- Joins content with newline character
- Appends new line to the result
- Handles memory management for intermediate strings
Returns: Updated content string or NULL if allocation fails
*/
static char	*handle_line_input(char *content, char *line)
{
	char	*temp;
	char	*new_content;

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
Processes heredoc input by collecting lines until delimiter is found:
- Takes delimiter and initial content as parameters
- Uses readline to get input line by line
- Appends newline after each line
- Stops when delimiter is encountered or EOF (Ctrl+D)
Returns: Complete heredoc content as a single string with newlines
*/
static char	*collect_heredoc_content(char *delimiter, char *content)
{
	char	*line;
	char	*new_content;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			return (content);
		}
		new_content = handle_line_input(content, line);
		if (!new_content)
			return (NULL);
		content = new_content;
	}
}

/*
Extracts initial heredoc content from input string until delimiter:
- Iterates through args starting from index i
- Keeps track of content length while searching
- If newline followed by delimiter is found, stops before delimiter
- Creates substring from start position with calculated length
Returns: Substring containing heredoc content, or NULL if allocation fails
Note: Updates i to point after the newline if delimiter is found
*/

static char	*get_first_heredoc_content(char *args, size_t *i, \
											char *delim, size_t start)
{
	size_t	len;

	len = 0;
	if (args[start] == ' ')
		start++;
	while (args[*i])
	{
		if (args[*i] == '\n' && ft_strncmp(&args[*i + 1], \
				delim, ft_strlen(delim)) == 0)
			break ;
		(*i)++;
		len++;
	}
	return (ft_substr(args, start, len));
}

/*
Handles the extraction and creation of heredoc tokens:
- Searches for delimiter in the input string
- If delimiter not found in args, prompts for input using collect_heredoc_content
- Creates a new token with the heredoc content
- Updates index to skip over the processed content
Returns: SUCCESS or ERR_MALLOC if memory allocation fails
*/

t_error	handle_heredoc(t_token **token_list, char *delim, size_t *i, char *args)
{
	char	*content;
	size_t	start;
	char	*newline_pos;
	t_error	result;

	start = *i;
	content = get_first_heredoc_content(args, i, delim, start);
	if (!content)
		return (ERR_MALLOC);
	newline_pos = ft_strchr(args + *i, '\n');
	if (!(newline_pos && ft_strncmp(newline_pos + 1, \
			delim, ft_strlen(delim)) == 0))
	{
		content = collect_heredoc_content(delim, content);
	}
	result = add_token(token_list, content, TYPE_HEREDOC_CONTENT);
	free(content);
	if (newline_pos)
		*i += ft_strlen(delim) + 1;
	return (result);
}
