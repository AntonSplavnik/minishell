/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:11:58 by abillote          #+#    #+#             */
/*   Updated: 2025/02/12 12:25:43 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Built-in env command implementation.
- If more than 1 arg, return an error as env does not take any options nor args.
- Go through the env_list and print each key and value.
*/
t_error	execute_env(char **args, t_env *env_list, t_shell *s)
{
	int	count_args;

	count_args = 0;
	while (args[count_args])
		count_args++;
	if (count_args > 1)
	{
		s->exit_status = 1;
		print_error_builtin("env", args[1], ": No such file or directory");
		return (ERR_ENV_EXEC);
	}
	while (env_list)
	{
		ft_putstr_fd(env_list->content, 1);
		write(1, "\n", 1);
		env_list = env_list->next;
	}
	s->exit_status = 0;
	return (SUCCESS);
}
