/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:27:25 by abillote          #+#    #+#             */
/*   Updated: 2025/01/01 14:14:59 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>

t_error handle_heredoc(t_token **token_list, char *delimiter, size_t *i, char *args)
{
    t_token	*heredoc_token;
	char    *content;
	size_t	start;
	size_t	len;

	start = *i;
	len = 0;
	while(args[*i] && ft_strcmp(args + *i, delimiter) != 0)
	{
		(*i)++;
		len++;
	}
	content = ft_substr(args, start, len);
	if (!content)
		return (ERR_MALLOC);
	heredoc_token = create_token(content, TYPE_HEREDOC_CONTENT);
	free(content);
	if (!heredoc_token)
		return (ERR_MALLOC);
	(*i) += ft_strlen(delimiter);
	return (add_token(token_list, heredoc_token->content, heredoc_token->type));
}
