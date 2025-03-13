/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:27:25 by abillote          #+#    #+#             */
/*   Updated: 2025/03/13 12:26:20 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>

/*
** Collects heredoc content until delimiter is encountered
*/
static char	*collect_heredoc_content(char *delimiter, char *initial_content)
{
	char	*line;
	char	*content;
	char	*new_content;

	if (!initial_content)
		content = ft_strdup("");
	else
		content = initial_content;
	if (!content)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
			return (handle_heredoc_cleanup(content, line));
		new_content = handle_line_input(content, line);
		if (!new_content)
		{
			free(line);
			free(content);
			return (NULL);
		}
		content = new_content;
	}
}

/*
 * Helper function to add a line to existing content
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

t_error	handle_heredoc(t_token **token_list, char *delim)
{
	char			*content;
	t_error			result;
	t_heredoc_info	*heredoc_info;
	t_token_type	content_type;

	content = NULL;
	heredoc_info = get_heredoc_info(delim);
	if (!heredoc_info)
		return (ERR_MALLOC);
	content = collect_heredoc_content(heredoc_info->delim, NULL);
	if (!content)
	{
		free(heredoc_info->delim);
		free(heredoc_info);
		return (ERR_MALLOC);
	}
	if (heredoc_info->is_quoted)
		content_type = TYPE_HEREDOC_CONTENT_QUOTED;
	else
		content_type = TYPE_HEREDOC_CONTENT;
	result = add_token(token_list, content, content_type);
	return (result);
}

/*
** Process a single heredoc based on whether it's the last one
*/
static t_error	process_single_heredoc(t_token **token_list,
	t_token *delim_token, int is_last)
{
	t_error	result;
	char	*content;

	if (is_last)
	{
		result = handle_heredoc(token_list, delim_token->content);
		if (result != SUCCESS)
			return (result);
	}
	else
	{
		content = collect_heredoc_content(delim_token->content, NULL);
		if (content)
			free(content);
	}
	return (SUCCESS);
}

/*
** Process all heredocs for a command but only store the last one's content
*/
t_error	process_heredocs(t_token **token_list)
{
	int		heredoc_count;
	t_token	*delim_tokens[MAX_HEREDOCS];
	int		i;
	t_error	result;

	heredoc_count = find_heredoc_delimiters(*token_list, delim_tokens);
	if (heredoc_count == 0)
		return (SUCCESS);
	i = 0;
	while (i < heredoc_count)
	{
		result = process_single_heredoc(token_list, delim_tokens[i], \
			(i == heredoc_count - 1));
		if (result != SUCCESS)
			return (result);
		i++;
	}
	return (SUCCESS);
}
