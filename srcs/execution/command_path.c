/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:57:49 by abillote          #+#    #+#             */
/*   Updated: 2025/03/13 13:07:56 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
