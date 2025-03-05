/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:39:56 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/05 14:13:26 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	free_resources
	handle_malloc_error
	is_builtin
	handle_exit_status
*/

/*
Purpose:
Frees allocated resources and updates the shell’s exit status if an error occurred.

Functionality:
Frees the allocated cmd_path.
If result indicates failure, updates s->exit_status to 1.
*/
t_error free_resources(char *cmd_path, t_shell *s, t_error result)
{
	free(cmd_path);
	if (result != SUCCESS)
		s->exit_status = 1;
	return (result);
}

/*
Purpose:
Handles memory allocation failures by setting the shell’s exit status.

Functionality:
Sets s->exit_status to 1.
Returns a memory allocation error (ERR_MALLOC).
*/
static t_error handle_malloc_error(t_shell *s)
{
	s->exit_status = 1;
	return (ERR_MALLOC);
}

/*
Purpose:
Determines whether a given command is a built-in shell command.

Functionality:
Compares cmd with a predefined list of built-in commands (echo, cd, pwd, export, unset, env, exit).
Returns 1 if cmd is a built-in command, otherwise returns 0.

*/
int is_builtin(char *cmd)
{
	const char	*builtins[7] = {"echo", "cd", "pwd", "export", \
									"unset", "env", "exit", NULL};
	int	i;
	int	count;

	i = 0;
	count = sizeof(builtins) / sizeof(builtins[0]);
	while (i < count)
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

/*
Purpose:
Handles exit statuses from child processes and signals.

Functionality:

If SIGINT was received, sets s->exit_status to 130.

If SIGQUIT was received, sets s->exit_status to 131 and prints a newline.

If the process exited normally, updates s->exit_status to its return value.
*/
void handle_exit_status(int status, t_shell *s)
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
