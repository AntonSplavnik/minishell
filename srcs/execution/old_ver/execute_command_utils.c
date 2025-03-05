/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:45:40 by asplavni          #+#    #+#             */
/*   Updated: 2025/02/28 13:48:12 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void handle_exit_status(int status, t_shell *s)
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

t_error free_resourses(char *cmd_path, char **args, t_shell *s, t_error result)
{
	free(cmd_path);
	if(result != SUCCESS)
		s->exit_status = 1;
	return (result);
}

static t_error	handle_malloc_error(t_shell *s)
{
	s->exit_status = 1;
	return (ERR_MALLOC);
}