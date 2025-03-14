/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:54:55 by abillote          #+#    #+#             */
/*   Updated: 2025/03/14 12:07:14 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Extracts unquoted string until next delimiter or whitespace:
- Tracks position with index i
- Returns substring from start position
- Sets error to ERR_MALLOC if memory allocation fails
*/
char	*extract_unquoted_token(const char *str, size_t *i, t_error *error)
{
	size_t	start;
	size_t	len;
	char	*token;

	start = *i;
	len = 0;
	while (str[*i] && !isspace(str[*i]) && !is_delimiter(str[*i]))
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
	if (ft_strcmp(token, "|") && (str[j] == '<' || \
		str[j] == '>' || str[j] == '|'))
	{
		*error = ERR_PARSING;
		free(token);
		return (NULL);
	}
	return (token);
}

/*
Helper function to extract content without quotes
Returns allocated string with content between quotes
*/
char	*extract_content_without_quotes(char *token, size_t *len)
{
	size_t	token_len;
	char	*content;

	token_len = ft_strlen(token);
	if (token_len >= 2 && \
		((token[0] == '"' && token[token_len - 1] == '"') || \
		(token[0] == '\'' && token[token_len - 1] == '\'')))
	{
		*len = token_len - 2;
		content = malloc(*len + 1);
		if (!content)
			return (NULL);
		ft_strncpy(content, token + 1, *len);
		content[*len] = '\0';
		return (content);
	}
	else
	{
		*len = token_len;
		return (ft_strdup(token));
	}
}

/*
Extract quoted token and handle adjacent quotes/special chars
*/
char	*extract_quoted_token(char *args, size_t *i, t_error *error)
{
	char	*token;
	char	*new_token;

	token = extract_quoted_token_helper(args, i, error);
	if (!token)
		return (NULL);
	token = handle_dollar_after_quote(args, i, token, error);
	if (!token)
		return (NULL);
	new_token = handle_adjacent_quotes(args, i, token, error);
	if (new_token)
	{
		free(token);
		return (new_token);
	}
	return (token);
}

/*
Main token extraction function:
- Skips leading whitespace
- Identifies token type and calls appropriate extracter
- Updates index to track position in input string
Returns: SUCCESS or error status if extraction fails
*/
t_error	ft_split_token(t_token **token_list, char *args, \
							size_t *i, char **token)
{
	t_error	error;
	int		is_export_with_equals;

	error = SUCCESS;
	is_export_with_equals = (*token_list \
			&& ft_strcmp((*token_list)->content, "export") == 0 \
			&& ft_strchr(args + *i, '='));
	while (args[*i] && is_space(args[*i]))
		(*i)++;
	if (!args[*i])
	{
		*token = NULL;
		return (SUCCESS);
	}
	if (is_export_with_equals)
		*token = extract_export_token(args, i, &error);
	else if ((args[*i] == '"' || args[*i] == '\''))
		*token = extract_quoted_token(args, i, &error);
	else if (is_delimiter(args[*i]))
		*token = extract_delimiter_token(args, i, &error);
	else
		*token = extract_unquoted_token(args, i, &error);
	if (error != SUCCESS)
		return (handle_error_free_tokens(error, token_list, NULL));
	return (SUCCESS);
}
