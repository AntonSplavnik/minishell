/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:27:25 by abillote          #+#    #+#             */
/*   Updated: 2024/12/06 16:34:45 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>

static char *collect_heredoc_content(char *delimiter)
{
    char    *line;
    char    *content;
    char    *temp;

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
        content = ft_strjoin(content, line);
        free(temp);
        temp = content;
        content = ft_strjoin(content, "\n");
        free(temp);
        free(line);
    }
    return (content);
}

t_error handle_heredoc(t_token **token_list, char *delimiter)
{
    t_token *heredoc_token;
    char    *content;

    content = collect_heredoc_content(delimiter);
    if (!content)
        return (ERR_MALLOC);

    heredoc_token = create_token(content, TYPE_HEREDOC_OP);
    free(content);
    if (!heredoc_token)
        return (ERR_MALLOC);

    return (add_token(token_list, heredoc_token->content, heredoc_token->type));
}
