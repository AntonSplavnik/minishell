/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:57:49 by abillote          #+#    #+#             */
/*   Updated: 2025/01/24 16:02:52 by abillote         ###   ########.fr       */
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

static char *search_in_paths(char **path_dirs, char *cmd)
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
if command contains a "/", it's treated
as a direct path and just duplicate it.
Otherwise:
- Searches for the path env var with get var value function,
usually something like "/usr/local/bin:/usr/bin:/bin"
- Splits the path is sub-directory:
	- would be path_dirs[0] = /usr/local/bin ||
path_dirs[1] = /usr/bin || path_dirs[2] = /bin
- Searches for executable in each PATH directories.
- Returns full path if found, NULL if not found
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
