/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:26:37 by abillote          #+#    #+#             */
/*   Updated: 2025/02/12 15:23:47 by abillote         ###   ########.fr       */
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
Built-in cd command implementation
- Store the old path value to update the env var OLDPWD later
- if cd does not have any args, get HOME value as "cd" w/o args redirect to HOME
- else, get the path with the next token content
- change directory with chdir. If successful, returns 0.
- update the PWD and OLDPWD env var in the env_list.
Returns: SUCCESS or error code
*/
t_error	execute_cd(t_token *cmd, t_shell *s)
{
	char	*path;
	char	*old_path;
	t_error	error;

	old_path = getcwd(NULL, 0);
	if (!old_path)
		return (ERR_EXEC);
	if (!cmd->next || cmd->next->type != TYPE_ARG)
		path = get_var_value(s->env_list, "HOME", 0, 0);
	else
		path = cmd->next->content;
	if (chdir(path) != 0)
	{
		free(old_path);
		s->exit_status = 1;
		print_error_builtin("cd", cmd->next->content, ": No such directory");
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
