/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_quotes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:45:32 by abillote          #+#    #+#             */
/*   Updated: 2025/02/12 11:16:54 by abillote         ###   ########.fr       */
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
