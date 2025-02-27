/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:57:49 by abillote          #+#    #+#             */
/*   Updated: 2025/02/26 17:57:09 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
- Searches for executable in PATH directories:
	- look for the full path (= path_dir[i] + / + command name).
	- check if this path with "access":
		- The flag F_OK check if the file exits.
		- The flag X_OK gives execute permission.
		- If no permission, exit status is 126 (found but not executable)
- Returns full path if found, NULL if not found
*/

char	*try_path_dir(char *dir, char *cmd, t_shell *s)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	if (!full_path)
		return (NULL);
	if (access(full_path, F_OK) == 0)
	{
		if (access(full_path, X_OK) == 0)
			return (full_path);
		else
		{
			s->exit_status = 126;
			free(full_path);
			return (NULL);
		}
	}
	free(full_path);
	return (NULL);
}

/*
Searches for executable in PATH directories:
- Iterates through each directory in PATH
- Attempts to find executable using try_path_dir
- If executable is not found, set exit status to 127
- Handles memory cleanup of path_dirs array
Returns: Full path to executable if found, NULL otherwise
*/
static char	*search_in_paths(char **path_dirs, char *cmd, t_shell *s)
{
	int		i;
	char	*full_path;

	i = 0;
	while (path_dirs[i])
	{
		full_path = try_path_dir(path_dirs[i], cmd, s);
		if (full_path)
		{
			free_array(path_dirs);
			return (full_path);
		}
		if (s->exit_status != 126)
			s->exit_status = 127;
		i++;
	}
	free_array(path_dirs);
	return (NULL);
}

/*
Check if the command file exist.
If it does not exist, set exit status to 127.
If it does exit, check execution right.
If no execution rights, set exit status to 126.
Returns:
- NULL if file is not found or not executable
- direct path on success
*/
char	*check_direct_path(char *cmd, t_shell *s)
{
	if (access(cmd, F_OK) != 0)
	{
		s->exit_status = 127;
		return (NULL);
	}
	if (access(cmd, X_OK) != 0)
	{
		s->exit_status = 126;
		return (NULL);
	}
	return (ft_strdup(cmd));
}

/*
Locates full path for a command:
- Handles direct paths (containing '/')
- If cmd file does not exist, set exit status to 127
- If cmd file exists but permission is denied, set exit status to 126
- Gets PATH variable from environment
- Splits PATH into directory list
- Searches directories for executable
Returns: Full path to command if found, NULL if not found or error
*/
char	*find_command_path(char *cmd, t_shell *s)
{
	char	*path_env;
	char	**path_dirs;

	if (ft_strchr(cmd, '/'))
		return (check_direct_path(cmd, s));
	path_env = get_var_value(s->env_list, "PATH", 0, 0);
	if (ft_strcmp(path_env, "") == 0)
	{
		s->exit_status = 127;
		return (NULL);
	}
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (NULL);
	return (search_in_paths(path_dirs, cmd, s));
}
