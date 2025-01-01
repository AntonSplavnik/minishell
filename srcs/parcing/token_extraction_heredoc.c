/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:27:25 by abillote          #+#    #+#             */
/*   Updated: 2025/01/01 14:41:05 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>

static char *collect_heredoc_content(char *delimiter)
{
	char *line;
	char *content;
	char *temp;

	content = ft_strdup("");
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		temp = content;
		content = ft_strjoin(temp, line);
		free(temp);
		temp = content;
		content = ft_strjoin(temp, "\n");
		free(temp);
		free(line);
	}
	return (content);
}

t_error handle_heredoc(t_token **token_list, char *delimiter, size_t *i, char *args)
{
	t_token	*heredoc_token;
	char	*content;
	size_t	start;
	size_t	len;

	start = *i;
	len = 0;
	while(args[*i] && ft_strncmp(args + *i , delimiter, ft_strlen(delimiter)) != 0)
	{
		(*i)++;
		len++;
	}
	content = ft_substr(args, start, len);
	if (!content)
		return (ERR_MALLOC);
	//checker if second delimiter exit - si pas de second delimiter, redonner le prompt jusqu'a ce qu'il y en ait un
	if (ft_strstr(args + start, delimiter) == 0)
		content = collect_heredoc_content(delimiter);
	heredoc_token = create_token(content, TYPE_HEREDOC_CONTENT);
	free(content);
	if (!heredoc_token)
		return (ERR_MALLOC);
	(*i) += ft_strlen(delimiter);
	return (add_token(token_list, heredoc_token->content, heredoc_token->type));
}
