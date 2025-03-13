/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:54:55 by abillote          #+#    #+#             */
/*   Updated: 2025/03/13 16:41:02 by abillote         ###   ########.fr       */
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
	if (str[j] == '<' || str[j] == '>' || str[j] == '|')
	{
		*error = ERR_PARSING;
		free(token);
		return (NULL);
	}
	return (token);
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
	else
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
	else if ((args[*i] == '"' || args[*i] == '\'') && is_space(args[*i - 1]))
		*token = extract_quoted_token(args, i, &error);
	else if (is_delimiter(args[*i]))
		*token = extract_delimiter_token(args, i, &error);
	else
		*token = extract_unquoted_token(args, i, &error);
	if (error != SUCCESS)
		return (handle_error_free_tokens(error, token_list, NULL));
	return (SUCCESS);
}
