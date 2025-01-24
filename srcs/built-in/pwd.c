/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 17:16:13 by abillote          #+#    #+#             */
/*   Updated: 2025/01/24 17:36:26 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Count the number of args.
If more than 1, return an error, as pwd takes no args.
Get the current working dir with getcwd.
By using getcwd(NULL, 0) => buffer is automatically
as big as needed to store the path (cd man getcwd)
Print the current dir path.
*/
t_error	execute_pwd(char **args)
{
	char	*current_dir;
	int		count_args;

	count_args = 0;
	while (args[count_args])
		count_args++;
	if (count_args > 1)
		return (ERR_PWD);
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (ERR_EXEC);
	ft_putstr_fd(current_dir, 1);
	write(1, "\n", 1);
	free(current_dir);
	return (SUCCESS);
}
