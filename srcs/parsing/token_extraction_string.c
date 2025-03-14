/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_string.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:04:24 by abillote          #+#    #+#             */
/*   Updated: 2025/03/14 12:07:18 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_if_exist(void *ptr1, void *ptr2)
{
	if (ptr1)
		free(ptr1);
	if (ptr2)
		free(ptr2);
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

	content1 = extract_content_without_quotes(token1, &len1);
	content2 = extract_content_without_quotes(token2, &len2);
	if (!content1 || !content2)
	{
		free_if_exist(content1, content2);
		return (NULL);
	}
	result = malloc(len1 + len2 + 1);
	if (!result)
	{
		free(content1);
		free(content2);
		return (NULL);
	}
	ft_strcpy(result, content1);
	ft_strcpy(result + len1, content2);
	free(content1);
	free(content2);
	return (result);
}

/*
Process more adjacent quotes if present
*/
char	*process_more_adjacent_quotes(char *args, size_t *i, \
				char *combined, t_error *error)
{
	char	*temp;

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

/*
Helper function to handle adjacent quoted strings
*/
char	*handle_adjacent_quotes(char *args, size_t *i, \
			char *token, t_error *error)
{
	char	*next_token;
	char	*combined;

	if (args[*i] && (args[*i] == '"' || args[*i] == '\''))
	{
		next_token = extract_quoted_token_helper(args, i, error);
		if (!next_token)
			return (NULL);
		combined = concatenate_tokens(token, next_token);
		free(next_token);
		if (!combined)
		{
			*error = ERR_MALLOC;
			return (NULL);
		}
		return (process_more_adjacent_quotes(args, i, combined, error));
	}
	return (NULL);
}

/*
Handle special $ character after quote extraction
*/
char	*handle_dollar_after_quote(char *args, size_t *i, \
			char *token, t_error *error)
{
	char	*new_token;
	size_t	len;

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
	return (token);
}
