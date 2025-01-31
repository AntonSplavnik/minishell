/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:06:52 by abillote          #+#    #+#             */
/*   Updated: 2025/01/13 17:50:41 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Checks if character is any type of whitespace
Returns: 1 if space character, 0 otherwise
*/
int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || \
			c == '\v' || c == '\f' || c == '\r');
}

/*
Checks if character is a shell special delimiter (<, >, |)
Returns: 1 if delimiter, 0 otherwise
*/
int	is_delimiter(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

/*
Determines length of delimiter at start of string:
- Returns 2 for '<<' or '>>'
- Returns 1 for single character delimiters
-Returns 0 if no delimiter or empty string
*/
int	get_delimiter_len(const char *str)
{
	if (!str || !*str)
		return (0);
	if ((*str == '<' && *(str + 1) == '<') || \
		(*str == '>' && *(str + 1) == '>'))
		return (2);
	if (is_delimiter(*str))
		return (1);
	return (0);
}

/*
Returns pointer to last token in the linked list
Returns NULL if list is empty
*/
t_token	*get_last_token(t_token *token_list)
{
	if (!token_list)
		return (NULL);
	while (token_list->next)
		token_list = token_list->next;
	return (token_list);
}

/*
Determines token type based on content:
- Handles quotes (single and double)
- Handles redirections (<, >, >>, <<)
- Handles pipes (|)
- Returns TYPE_ARG for regular arguments
*/
t_token_type	get_other_types(char *input)
{
	if (ft_strcmp(input, "\"") == 0)
		return (TYPE_DQUOTE);
	if (ft_strcmp(input, "'") == 0)
		return (TYPE_SQUOTE);
	if (ft_strcmp(input, "|") == 0)
		return (TYPE_PIPE);
	if (ft_strcmp(input, "<") == 0)
		return (TYPE_REDIRIN);
	if (ft_strcmp(input, ">") == 0)
		return (TYPE_REDIROUT);
	if (ft_strcmp(input, ">>") == 0)
		return (TYPE_REDIRAPPEND);
	if (ft_strcmp(input, "<<") == 0)
		return (TYPE_HEREDOC_OP);
	if (ft_strcmp(input, "") == 0)
		return (TOKEN_EMPTY);
	else
		return (TYPE_ARG);
}
