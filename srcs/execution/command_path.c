/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:57:49 by abillote          #+#    #+#             */
/*   Updated: 2025/01/24 16:08:06 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
- Searches for executable in PATH directories:
	- look for the full path (= path_dir[i] + / + command name).
	- check if this path is executable with "access".
	- The flag X_OK gives execute permission.
- Returns full path if found, NULL if not found
*/

char	*try_path_dir(char *dir, char *cmd)
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
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

/*
Searches for executable in PATH directories:
- Iterates through each directory in PATH
- Attempts to find executable using try_path_dir
- Handles memory cleanup of path_dirs array
Returns: Full path to executable if found, NULL otherwise
*/
static char	*search_in_paths(char **path_dirs, char *cmd)
{
	int		i;
	char	*full_path;

	i = 0;
	while (path_dirs[i])
	{
		full_path = try_path_dir(path_dirs[i], cmd);
		if (full_path)
		{
			free_array(path_dirs);
			return (full_path);
		}
		i++;
	}
	free_array(path_dirs);
	return (NULL);
}

/*
Locates full path for a command:
- Handles direct paths (containing '/')
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
		return (ft_strdup(cmd));
	path_env = get_var_value(s->env_list, "PATH", 0, 0);
	if (ft_strcmp(path_env, "") == 0)
		return (NULL);
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (NULL);
	return (search_in_paths(path_dirs, cmd));
}
