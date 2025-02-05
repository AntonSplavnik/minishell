/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:27:25 by abillote          #+#    #+#             */
/*   Updated: 2025/02/05 14:51:21 by abillote         ###   ########.fr       */
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
Processes heredoc content and updates position:
- Checks for newline and delimiter in input
- Collects additional content if needed
- Updates index position after processing
Returns: SUCCESS
*/
static t_error	process_heredoc_content(char **content, \
					t_heredoc_info *heredoc_info, char *args, size_t *i)
{
	char	*newline_pos;

	newline_pos = ft_strchr(args + *i, '\n');
	if (!(newline_pos && ft_strncmp(newline_pos + 1, \
		heredoc_info->delim, ft_strlen(heredoc_info->delim)) == 0))
		*content = collect_heredoc_content(heredoc_info->delim, *content);
	if (newline_pos)
		*i += ft_strlen(heredoc_info->delim) + 1;
	return (SUCCESS);
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
- Check if delimiter is quoted or not -
add a flag and remove the quotes if quoted
- Searches for the unquoted delimiter in the input string (= args)
- If delimiter not found in args, prompts for input
 using process_heredoc_content
- Creates appropriate token type based on quoted status
- Creates a new token with the heredoc content
- Updates index to skip over the processed content
Returns: SUCCESS or ERR_MALLOC if memory allocation fails
*/

t_error	handle_heredoc(t_token **token_list, char *delim, size_t *i, char *args)
{
	char			*content;
	size_t			start;
	t_error			result;
	t_heredoc_info	*heredoc_info;
	t_token_type	content_type;

	start = *i;
	heredoc_info = get_heredoc_info(delim);
	if (!heredoc_info)
		return (ERR_MALLOC);
	content = get_first_heredoc_content(args, i, heredoc_info->delim, start);
	if (!content)
		return (cleanup_heredoc(NULL, heredoc_info, ERR_MALLOC));
	if (process_heredoc_content(&content, heredoc_info, args, i) != SUCCESS)
		return (cleanup_heredoc(content, heredoc_info, ERR_MALLOC));
	if (heredoc_info->is_quoted)
		content_type = TYPE_HEREDOC_CONTENT_QUOTED;
	else
		content_type = TYPE_HEREDOC_CONTENT;
	result = add_token(token_list, content, content_type);
	cleanup_heredoc(content, heredoc_info, SUCCESS);
	return (result);
}
