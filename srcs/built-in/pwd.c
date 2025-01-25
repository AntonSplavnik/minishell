/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 17:16:13 by abillote          #+#    #+#             */
/*   Updated: 2025/01/25 21:39:18 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Built-in pwd command implementation
- Get the current working dir with getcwd.
By using getcwd(NULL, 0) => buffer is automatically
as big as needed to store the path (cd man getcwd)
- Print the current dir path.
*/
t_error	execute_pwd(t_shell *s)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		s->exit_status = 1;
		return (ERR_EXEC);
	}
	ft_putstr_fd(current_dir, 1);
	write(1, "\n", 1);
	free(current_dir);
	s->exit_status = 0;
	return (SUCCESS);
}
