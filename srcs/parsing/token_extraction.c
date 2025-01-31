/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:54:55 by abillote          #+#    #+#             */
/*   Updated: 2025/01/31 17:15:25 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Extracts unquoted string until next delimiter or whitespace:
- Tracks position with index i
- Returns substring from start position
- Sets error to ERR_MALLOC if memory allocation fails
*/
static char	*extract_unquoted_token(const char *str, size_t *i, t_error *error)
{
	size_t	start;
	size_t	len;
	char	*token;

	start = *i;
	len = 0;
	while (str[*i] && !isspace(str[*i]) && !is_delimiter(str[*i]) && \
		str[*i] != '"' && str[*i] != '\'')
	{
		(*i)++;
		len++;
	}
	token = ft_substr(str, start, len);
	if (!token)
		*error = ERR_MALLOC;
	return (token);
}

/*
Extracts special shell delimiters (<, >, |, <<, >>):
- Gets delimiter length (1 or 2 characters)
- Creates substring containing delimiter
- Updates index to skip processed characters
Returns: Extracted delimiter or NULL on memory error
*/
static char	*extract_delimiter_token(const char *str, size_t *i, t_error *error)
{
	size_t	len;
	char	*token;
	size_t	j;

	len = get_delimiter_len(str + *i);
	token = ft_substr(str, *i, len);
	if (!token)
	{
		*error = ERR_MALLOC;
		return (NULL);
	}
	*i += len;
	j = *i;
	while (is_space(str[j]))
		j++;
	if (str[j] == '<' || str[j] == '>' || str[j] == '|')
	{
		*error = ERR_PARSING;
		free(token);
		return (NULL);
	}
	return (token);
}

/*
Processes quoted content to handle nested quotes:
- Tracks position with index i
- Counts length including both quote characters
- Updates quote status in quote_info structure
Returns: Length of quoted content including quotes
*/
static size_t	handle_quoted_content(char *args, size_t *i, \
				t_quote_info *quotes)
{
	size_t	len;

	len = 1;
	(*i)++;
	while (args[*i])
	{
		if (args[*i] == quotes->quote_char)
		{
			quotes->num_quote = 2;
			(*i)++;
			len++;
			break ;
		}
		(*i)++;
		len++;
	}
	return (len);
}

/*
Creates a token from quoted content:
- Handles both single and double quotes
- Validates matching closing quotes
- Returns NULL if quotes are unmatched
- Sets error to ERR_PARSING for quote errors
Returns: Token string including quotes or NULL on error
*/
static char	*extract_quoted_token(char *args, size_t *i, t_error *error)
{
	t_quote_info	quotes;
	size_t			start;
	size_t			len;
	char			*token;

	quotes.quote_char = args[*i];
	start = *i;
	quotes.num_quote = 1;
	len = handle_quoted_content(args, i, &quotes);
	if (len == 1 || quotes.num_quote != 2)
	{
		*error = ERR_PARSING;
		return (NULL);
	}
	if (len == 2 && (ft_isalnum(args[*i]) || args[*i] == '$'))
	{
		len = 0;
		return (extract_unquoted_token(args, i, error));
	}
	token = ft_substr(args, start, len);
	if (!token)
	{
		*error = ERR_MALLOC;
		return (NULL);
	}
	return (token);
}

/*
Main token extraction function:
- Skips leading whitespace
- Identifies token type (quoted, delimiter, or unquoted)
- Calls appropriate extraction function
- Updates index to track position in input string
Returns: SUCCESS or error status if extraction fails
*/
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
