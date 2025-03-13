/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:39:56 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/13 13:07:11 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	free_resources(char *cmd_path, t_shell *s, t_error result)
{
	free(cmd_path);
	if (result != SUCCESS)
		s->exit_status = 1;
	return (result);
}

t_error	handle_malloc_error(t_shell *s)
{
	s->exit_status = 1;
	return (ERR_MALLOC);
}

void	handle_exit_status(int status, t_shell *s)
{
	if (g_sig == SIGINT)
		s->exit_status = 130;
	else if (g_sig == SIGQUIT)
	{
		s->exit_status = 131;
		write(1, "\n", 1);
	}
	else if (WIFEXITED(status))
		s->exit_status = WEXITSTATUS(status);
}
