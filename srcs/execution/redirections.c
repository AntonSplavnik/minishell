/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:10:08 by asplavni          #+#    #+#             */
/*   Updated: 2025/02/26 17:57:12 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error execute_redirect(t_token *cmd_token, t_shell *s)
{
	t_token *current;
	t_error	result;

	current = s->token_list;
	while (current)
	{
		if (ft_strcmp(current->content, "<") == 0)
			result = input_redirection(s);
		if (ft_strcmp(current->content, ">") == 0)
			result = output_redirection(s);
		if (ft_strcmp(current->content, "<<") == 0)
			result = here_document(s);
		if (ft_strcmp(current->content, ">>") == 0)
			result = append_output_redirection(s);
		current = current->next;
	}
	return (result);
}

t_error input_redirection(t_shell *s)
{

}

t_error output_redirection(t_shell *s)
{

}
t_error here_document(t_shell *s)
{

}
t_error append_output_redirection(t_shell *s)
{

}