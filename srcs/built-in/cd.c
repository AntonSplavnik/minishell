/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:26:37 by abillote          #+#    #+#             */
/*   Updated: 2025/03/07 12:43:23 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Updates PWD and OLDPWD environment variables after directory change
Returns: SUCCESS or ERR_ENV if update fails
*/
t_error	update_path_vars(char *old_path, t_env *env_list)
{
	char	*current_path;
	t_env	*pwd_var;
	t_env	*oldpwd_var;

	current_path = getcwd(NULL, 0);
	if (!current_path)
		return (ERR_ENV);
	pwd_var = find_env_var(env_list, "PWD");
	if (!pwd_var)
		return (ERR_ENV);
	free(pwd_var->value);
	pwd_var->value = ft_strdup(current_path);
	free(current_path);
	oldpwd_var = find_env_var(env_list, "OLDPWD");
	if (!oldpwd_var)
		return (ERR_ENV);
	free(oldpwd_var->value);
	oldpwd_var->value = ft_strdup(old_path);
	return (SUCCESS);
}

/*
* Handles the directory change process for the cd command
* - Takes path from args or HOME environment variable
* - Attempts to change directory with chdir
* - Updates environment variables if successful
* Returns: SUCCESS or appropriate error code
*/
static t_error	change_directory(char *path, char *old_path, t_shell *s)
{
	t_error	error;

	if (chdir(path) != 0)
	{
		free(old_path);
		s->exit_status = 1;
		if (path)
			print_error_builtin("cd", path, ": No such file or directory");
		else
			print_error_builtin("cd", "", ": No such file or directory");
		return (ERR_CD);
	}
	error = update_path_vars(old_path, s->env_list);
	if (error != SUCCESS)
		s->exit_status = 1;
	else
		s->exit_status = 0;
	free(old_path);
	return (error);
}

/*
Built-in cd command implementation
- Validates number of arguments
- Gets current working directory for OLDPWD
- Determines target path based on arguments
- Delegates to change_directory for the actual directory change
Returns: SUCCESS or error code
 */
t_error	execute_cd(char **args, t_token *cmd, t_shell *s)
{
	char	*path;
	char	*old_path;
	int		count_args;

	count_args = 0;
	while (args[count_args])
		count_args++;
	if (count_args > 2)
	{
		s->exit_status = 1;
		print_error_builtin("cd", "", ": too many arguments");
		return (ERR_CD);
	}
	old_path = getcwd(NULL, 0);
	if (!old_path)
		return (ERR_EXEC);
	if (count_args == 1)
		path = get_var_value(s->env_list, "HOME", 0, 0);
	else
		path = args[1];
	return (change_directory(path, old_path, s));
}
