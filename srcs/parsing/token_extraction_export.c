/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_export.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:13:16 by abillote          #+#    #+#             */
/*   Updated: 2025/02/12 11:17:03 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Process quoted content in export token
- Handles both single and double quotes in export arguments
- Detects start of quoted sections and updates quote status
- Detects end of quoted sections and resets quote status
- Handles spaces differently when inside or outside quotes
- Ensures proper parsing of quoted values in export assignments
Parameters:
 - args: Input string being processed
 - i: Current position in the input string
 - in_quotes: Pointer to flag indicating if currently in quoted section
 - quote_char: Pointer to character storing the current quote type
Returns:
 - 1: Continue processing current token
 - 0: End of token reached (unquoted space found)
*/
static int	process_export_quotes(char *args, size_t *i, int *in_quotes, \
									char *quote_char)
{
	if ((args[*i] == '"' || args[*i] == '\'') && !*in_quotes)
	{
		*quote_char = args[*i];
		*in_quotes = 1;
	}
	else if (args[*i] == *quote_char && *in_quotes)
		*in_quotes = 0;
	else if (is_space(args[*i]) && !*in_quotes)
		return (0);
	return (1);
}

/*
Handles special export case with equals sign
- Extracts complete export argument including quoted sections
- Properly handles variable assignments with spaces in values
- Maintains quote context throughout extraction
- Supports both single and double quoted values
- Handles error cases and memory allocation
Parameters:
 - args: Input string containing export command and arguments
 - i: Pointer to current position in input string (updated during processing)
 - error: Pointer to error status for reporting allocation failures
Returns:
 - Extracted token string or NULL if allocation fails
 - Updates error status if memory allocation fails
 - Updates position index to end of extracted token
*/
char	*extract_export_token(char *args, size_t *i, t_error *error)
{
	size_t	start;
	size_t	len;
	int		in_quotes;
	char	quote_char;
	char	*token;

	start = *i;
	len = 0;
	in_quotes = 0;
	quote_char = 0;
	while (args[*i])
	{
		if (!process_export_quotes(args, i, &in_quotes, &quote_char))
			break ;
		len++;
		(*i)++;
	}
	token = ft_substr(args, start, len);
	if (!token)
		*error = ERR_MALLOC;
	return (token);
}
