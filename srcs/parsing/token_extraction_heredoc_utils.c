/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_heredoc_utils.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 12:12:24 by abillote          #+#    #+#             */
/*   Updated: 2025/02/05 14:51:06 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Handles memory cleanup for heredoc processing:
- Frees heredoc content if provided
- Frees heredoc info structure and its members
*/
t_error	cleanup_heredoc(char *content, t_heredoc_info *info, t_error status)
{
	if (content)
		free(content);
	if (info)
	{
		free(info->delim);
		free(info);
	}
	return (status);
}

/*
Creates heredoc info structure:
- Sets quote status and processes delimiter
- Determines if delimiter is wrapped in quotes
- Store content without quotes if quoted
- Store original content if not quoted
- Handles memory cleanup on failure
Returns: Pointer to info structure or NULL if allocation fails
*/
t_heredoc_info	*get_heredoc_info(char *delim)
{
	t_heredoc_info	*heredoc_info;
	int				len;

	heredoc_info = malloc(sizeof(t_heredoc_info));
	if (!heredoc_info)
		return (NULL);
	len = ft_strlen(delim);
	heredoc_info->is_quoted = 0;
	if (len >= 2 && ((delim[0] == '\'' && delim[len - 1] == '\'') \
			|| (delim[0] == '"' && delim[len - 1] == '"')))
	{
		heredoc_info->is_quoted = 1;
		heredoc_info->delim = ft_strndup(delim + 1, len - 2);
	}
	else
		heredoc_info->delim = ft_strdup(delim);
	if (!heredoc_info->delim)
	{
		free(heredoc_info);
		return (NULL);
	}
	return (heredoc_info);
}

/*Returns pointer to static heredoc delimiter state:
- Uses static function to maintain encapsulation
- Provides controlled access to state
Returns: Pointer to static state variable
*/
int	*get_heredoc_state_ptr(void)
{
	static int	expect_heredoc_delim = 0;

	return (&expect_heredoc_delim);
}

/*
Resets parser state:
- Gets pointer to static state
- Sets state back to initial value
- Used for error recovery
*/
void	reset_heredoc_state(void)
{
	int	*expect_heredoc_delim;

	expect_heredoc_delim = get_heredoc_state_ptr();
	*expect_heredoc_delim = 0;
}
