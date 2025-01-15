/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_heredoc_utils.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 12:12:24 by abillote          #+#    #+#             */
/*   Updated: 2025/01/14 12:19:41 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
