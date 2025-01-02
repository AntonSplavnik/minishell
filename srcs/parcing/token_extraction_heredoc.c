/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:27:25 by abillote          #+#    #+#             */
/*   Updated: 2025/01/02 19:22:22 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>

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

t_error	handle_heredoc(t_token **token_list, char *delimiter, \
						size_t *i, char *args)
{
	t_token	*heredoc_token;
	char	*content;
	size_t	start;
	size_t	len;

	start = *i;
	len = 0;
	while (args[*i] && ft_strncmp(args + *i, delimiter, \
				ft_strlen(delimiter)) != 0)
	{
		(*i)++;
		len++;
	}
	content = ft_substr(args, start, len);
	if (!content)
		return (ERR_MALLOC);
	if (ft_strstr(args + start, delimiter) == 0)
		content = collect_heredoc_content(delimiter, content);
	heredoc_token = create_token(content, TYPE_HEREDOC_CONTENT);
	free(content);
	if (!heredoc_token)
		return (ERR_MALLOC);
	(*i) += ft_strlen(delimiter);
	return (add_token(token_list, heredoc_token->content, heredoc_token->type));
}

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
