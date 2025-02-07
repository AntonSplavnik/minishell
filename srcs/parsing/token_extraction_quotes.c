/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_quotes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:45:32 by abillote          #+#    #+#             */
/*   Updated: 2025/02/07 18:30:02 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Edge case: extract token such as ""hello"" > ignore the two double quotes
Do the same with double single quotes
*/
char	*extract_double_quotes(char *args, size_t *i, \
								t_error *error, char quote_char)
{
	char	*token;

	token = extract_unquoted_token(args, i, error);
	if (quote_char == args[*i] && quote_char == args[*i + 1])
		(*i) += 2;
	return (token);
}

/*
Helper function to process quotes in export edge case:
- Tracks quote status and updates length
- Updates position index
Returns: Length of processed content
*/
static size_t process_export_quotes(char *args, size_t *i, int *in_quotes, char *quote_char)
{
	size_t len;

	len = 0;
	while (args[*i])
	{
		if ((args[*i] == '"' || args[*i] == '\'') && !*in_quotes)
		{
			*quote_char = args[*i];
			*in_quotes = 1;
		}
		else if (args[*i] == *quote_char && *in_quotes)
		{
			*in_quotes = 0;
		}
		else if (is_space(args[*i]) && !*in_quotes)
			break;
		len++;
		(*i)++;
	}
	return (len);
}

/*
Extracts export edge case content:
- Handles quoted content in export arguments
- Creates substring from processed content
Returns: Extracted token or NULL if allocation fails
*/
char *extract_export_edge_case(char *args, size_t *i, t_error *error)
{
	size_t	start;
	int		in_quotes;
	char	quote_char;
	char	*token;
	size_t	len;

	start = *i;
	in_quotes = 0;
	quote_char = 0;
	len = process_export_quotes(args, i, &in_quotes, &quote_char);
	token = ft_substr(args, start, len);
	if (!token)
	{
		*error = ERR_MALLOC;
		return (NULL);
	}
	return (token);
}
