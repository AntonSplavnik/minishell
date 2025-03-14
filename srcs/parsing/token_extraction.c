/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:54:55 by abillote          #+#    #+#             */
/*   Updated: 2025/03/14 11:36:50 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:45:21 by abillote          #+#    #+#             */
/*   Updated: 2025/03/14 17:46:44 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Copies up to n characters from src to dest.
If src is less than n characters, the remainder of dest is filled with nulls.
*/
char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

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
	if (ft_strcmp(token, "|") && (str[j] == '<' || str[j] == '>' || str[j] == '|'))
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

	/* Check if token has quotes at both ends */
	if (token_len >= 2 &&
		((token[0] == '"' && token[token_len - 1] == '"') ||
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
		/* If no quotes, just duplicate the token */
		*len = token_len;
		return (ft_strdup(token));
	}
}

/*
Helper function to concatenate two tokens by removing quotes
and combining their contents
*/
char	*concatenate_tokens(char *token1, char *token2)
{
	char	*content1;
	char	*content2;
	char	*result;
	size_t	len1;
	size_t	len2;

	/* Extract content without quotes */
	content1 = extract_content_without_quotes(token1, &len1);
	content2 = extract_content_without_quotes(token2, &len2);

	if (!content1 || !content2)
	{
		if (content1)
			free(content1);
		if (content2)
			free(content2);
		return (NULL);
	}

	/* Allocate memory for combined string */
	result = malloc(len1 + len2 + 1);
	if (!result)
	{
		free(content1);
		free(content2);
		return (NULL);
	}

	/* Combine strings */
	ft_strcpy(result, content1);
	ft_strcpy(result + len1, content2);

	free(content1);
	free(content2);

	return (result);
}



/*
Helper function to handle adjacent quoted strings
Returns a new concatenated token if adjacent quotes are found,
NULL otherwise to indicate no concatenation needed
*/
char	*handle_adjacent_quotes(char *args, size_t *i, char *token, t_error *error)
{
	char	*next_token;
	char	*combined;
	char	*temp;

	/* Check if the next character is a quote */
	if (args[*i] && (args[*i] == '"' || args[*i] == '\''))
	{
		/* Extract the content of the next quoted string */
		next_token = extract_quoted_token_helper(args, i, error);
		if (!next_token)
			return (NULL); /* Error or no valid quoted string */

		/* Concatenate the tokens */
		combined = concatenate_tokens(token, next_token);
		free(next_token);

		if (!combined)
		{
			*error = ERR_MALLOC;
			return (NULL);
		}

		/* Check for more adjacent quotes recursively */
		if (args[*i] && (args[*i] == '"' || args[*i] == '\''))
		{
			temp = handle_adjacent_quotes(args, i, combined, error);
			if (temp)
			{
				free(combined);
				return (temp);
			}
		}

		return (combined);
	}

	return (NULL); /* No adjacent quotes */
}



/* Helper function to extract quoted content and handle $ if needed */
char	*extract_quoted_token(char *args, size_t *i, t_error *error)
{
	char	*token;
	char	*new_token;
	size_t	len;

	token = extract_quoted_token_helper(args, i, error);
	if (args[*i] == '$' && (!args[*i + 1] || is_space(args[*i + 1])))
	{
		len = ft_strlen(token);
		(*i)++;
		new_token = malloc(len + 2);
		if (!new_token)
		{
			free(token);
			*error = ERR_MALLOC;
			return (NULL);
		}
		ft_strcpy(new_token, token);
		new_token[len] = '$';
		new_token[len + 1] = '\0';
		free(token);
		return (new_token);
	}

	/* Handle adjacent quoted strings by concatenating them */
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
