/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_quotes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:45:32 by abillote          #+#    #+#             */
/*   Updated: 2025/03/13 16:26:13 by abillote         ###   ########.fr       */
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
Creates a token from quoted content:
- Handles both single and double quotes
- Validates matching closing quotes
- Returns NULL if quotes are unmatched
- Sets error to ERR_PARSING for quote errors
Returns: Token string including quotes or NULL on error
*/
char	*extract_quoted_token_helper(char *args, size_t *i, t_error *error)
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
		return (extract_double_quotes(args, i, error, quotes.quote_char));
	token = ft_substr(args, start, len);
	if (!token)
	{
		*error = ERR_MALLOC;
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
size_t	handle_quoted_content(char *args, size_t *i, \
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
