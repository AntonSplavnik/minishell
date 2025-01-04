/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:27:25 by abillote          #+#    #+#             */
/*   Updated: 2025/01/04 12:23:39 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>

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
	char	*temp;
	char	*new_line;

	while (1)
	{
		new_line = "\n";
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		temp = ft_strjoin(content, new_line);
		content = ft_strjoin(temp, line);
		free(temp);
		free(line);
	}
	return (content);
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
	char	*content;
	size_t	len;

	len = 0;
	while (args[*i])
	{
		if (args[*i] == '\n' && !ft_strncmp(&args[*i + 1], \
				delim, ft_strlen(delim)))
		{
			(*i)++;
			break ;
		}
		(*i)++;
		len++;
	}
	content = ft_substr(args, start, len);
	return (content);
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
	t_token	*heredoc_token;
	char	*content;
	size_t	start;
	char	*newline_pos;

	start = *i;
	content = get_first_heredoc_content(args, i, delim, start);
	if (!content)
		return (ERR_MALLOC);
	newline_pos = ft_strchr(args + start, '\n');
	if (!(newline_pos && ft_strncmp(newline_pos + 1, \
			delim, ft_strlen(delim)) == 0))
		content = collect_heredoc_content(delim, content);
	heredoc_token = create_token(content, TYPE_HEREDOC_CONTENT);
	free(content);
	if (!heredoc_token)
		return (ERR_MALLOC);
	*i += ft_strlen(delim);
	return (add_token(token_list, heredoc_token->content, heredoc_token->type));
}

/*
Checks if current token is part of a heredoc operation:
- Uses static variable to track heredoc state
- Returns TYPE_HEREDOC_DELIM if expecting delimiter
- Returns TYPE_HEREDOC_OP for '<<' operator
- Returns 0 for non-heredoc tokens
*/
int	check_if_heredoc(char *input)
{
	static int	expect_heredoc_delim = 0;

	if (expect_heredoc_delim)
	{
		expect_heredoc_delim = 0;
		return (TYPE_HEREDOC_DELIM);
	}
	if (ft_strcmp(input, "<<") == 0)
	{
		expect_heredoc_delim = 1;
		return (TYPE_HEREDOC_OP);
	}
	return (0);
}
