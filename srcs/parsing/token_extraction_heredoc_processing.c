/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_heredoc_processing.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/03/17 15:02:42 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>

/*
** Process a single line in heredoc collection
** Returns:
** - SUCCESS if line was processed
** - ERR_MALLOC if memory allocation failed
** - ERR_DELIMITER if delimiter was matched
** - ERR_SIGNAL if signal interrupted
*/
t_error	process_heredoc_line(char *line, char *delimiter, char **content_ptr)
{
	char	*new_content;

	if (!line || g_sig == SIGINT)
		return (ERR_SIGNAL);
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (ERR_DELIMITER);
	}
	new_content = handle_line_input(*content_ptr, line);
	if (!new_content)
	{
		free(line);
		free(*content_ptr);
		*content_ptr = NULL;
		return (ERR_MALLOC);
	}
	*content_ptr = new_content;
	return (SUCCESS);
}

/*
** Handle heredoc operation for a token
** Returns SUCCESS or appropriate error code
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
	result = collect_heredoc_content(heredoc_info->delim, &content);
	if (result == ERR_SIGNAL || g_sig == SIGINT)
	{
		cleanup_heredoc_info(heredoc_info, content);
		return (ERR_SIGNAL);
	}
	if (result != SUCCESS || !content)
	{
		cleanup_heredoc_info(heredoc_info, content);
		return (result);
	}
	content_type = get_heredoc_content_type(heredoc_info);
	result = add_token(token_list, content, content_type);
	cleanup_heredoc_info(heredoc_info, content);
	return (result);
}

/*
** Processes a single heredoc during multi-heredoc handling
** Returns SUCCESS or appropriate error code
*/
t_error	process_single_heredoc(t_token **token_list, t_token *delim_token,
								int is_last)
{
	t_error	result;
	char	*temp_content;

	if (is_last)
		return (handle_heredoc(token_list, delim_token->content));
	temp_content = NULL;
	result = collect_heredoc_content(delim_token->content, &temp_content);
	if (temp_content)
		free(temp_content);
	return (result);
}

/*
** Main entry point to process all heredocs in a command
** Processes all heredocs but only keeps the last one
** Returns SUCCESS or appropriate error code
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
										i == heredoc_count - 1);
		if (result == ERR_SIGNAL || g_sig == SIGINT)
		{
			set_signals_interactive();
			return (ERR_SIGNAL);
		}
		if (result != SUCCESS)
			return (result);
		i++;
	}
	return (SUCCESS);
}

/*
** Initialize and find all heredoc delimiters in the command
** Helper function already defined in token_extraction_heredoc_utils.c
*/
int	find_heredoc_delimiters(t_token *token_list,
							t_token *delim_tokens[MAX_HEREDOCS])
{
	int		heredoc_count;
	t_token	*current;

	heredoc_count = 0;
	current = token_list;
	while (current)
	{
		if (current->type == TYPE_HEREDOC_DELIM)
		{
			if (heredoc_count < MAX_HEREDOCS)
				delim_tokens[heredoc_count++] = current;
		}
		if (current->type == TYPE_PIPE)
			break ;
		current = current->next;
	}
	return (heredoc_count);
}
