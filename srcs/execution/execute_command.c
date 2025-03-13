/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:39:28 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/13 13:07:37 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	execute_command(t_shell *s)
{
	t_token	*cmd_copy;
	t_error	res;

	cmd_copy = copy_tokens(s->token_list);
	if (!cmd_copy)
		return (ERR_MALLOC);
	if (has_pipe(s))
		res = handle_pipe_operations(s);
	else
		res = execute_single_command(cmd_copy, s);
	token_clear(&cmd_copy);
	return (res);
}
