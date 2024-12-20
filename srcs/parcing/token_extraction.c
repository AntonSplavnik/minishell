/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:54:55 by abillote          #+#    #+#             */
/*   Updated: 2024/12/06 15:09:44 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//extract regular string
static char	*extract_unquoted_token(const char *str, size_t *i, t_error *error)
{
	size_t	start;
	size_t	len;

	start = *i;
	len = 0;
	while (str[*i] && !isspace(str[*i]) && !is_delimiter(str[*i]) && \
		str[*i] != '"' && str[*i] != '\'')
	{
		(*i)++;
		len++;
	}
	if (!ft_substr(str, start, len))
		*error = ERR_MALLOC;
	return (ft_substr(str, start, len));
}

//handle the delimiter (<, >, <<, >>, |)
static char	*extract_delimiter_token(const char *str, size_t *i, t_error *error)
{
	size_t	len;
	char	*token;

	len = get_delimiter_len(str + *i);
	token = ft_substr(str, *i, len);
	if (!token)
		*error = ERR_MALLOC;
	*i += len;
	return (token);
}

//Handle the quotes
static size_t	handle_quoted_content(char *args, size_t *i, \
				t_quote_info *quotes)
{
	size_t	len;

	len = 1;
	(*i)++;
	while (args[*i])
	{
		if (args[*i] == quotes->outer_quote)
		{
			quotes->num_outer_quote = 2;
			(*i)++;
			len++;
			break ;
		}
		(*i)++;
		len++;
	}
	return (len);
}

//Extract the quoted token
static char	*extract_quoted_token(char *args, size_t *i, t_error *error)
{
	t_quote_info	quotes;
	size_t			start;
	size_t			len;

	quotes.outer_quote = args[*i];
	start = *i;
	quotes.in_inner_quote = 0;
	quotes.inner_quote = 0;
	quotes.num_outer_quote = 1;
	len = handle_quoted_content(args, i, &quotes);
	if (len == 1 || quotes.num_outer_quote != 2)
	{
		*error = ERR_PARCING;
		return (NULL);
	}
	if (!ft_substr(args, start, len))
	{
		*error = ERR_MALLOC;
		return (NULL);
	}
	return (ft_substr(args, start, len));
}

//Split the next token by checking spaces, quotes and delimiters and store it
t_error	ft_split_token(t_token **token_list, char *args, \
						size_t *i, char **token)
{
	t_error	error;

	error = SUCCESS;
	while (args[*i] && is_space(args[*i]))
		(*i)++;
	if (!args[*i])
	{
		*token = NULL;
		return (SUCCESS);
	}
	if (args[*i] == '"' || args[*i] == '\'')
		*token = extract_quoted_token(args, i, &error);
	else if (is_delimiter(args[*i]))
		*token = extract_delimiter_token(args, i, &error);
	else
		*token = extract_unquoted_token(args, i, &error);
	if (error != SUCCESS)
		return (handle_error_free_tokens(error, token_list, NULL));
	return (SUCCESS);
}
