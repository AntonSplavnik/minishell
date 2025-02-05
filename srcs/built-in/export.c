/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:13:03 by abillote          #+#    #+#             */
/*   Updated: 2025/02/05 18:08:06 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Count number of environment variables in linked list
Returns: Total number of variables
*/
static size_t	count_env_var(t_env *env_list)
{
	size_t	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

/*
Sorts environment variables array alphabetically using bubble sort
*/
static void	sort_env_array(char **env_array)
{
	int		i;
	int		j;
	char	*temp;
	int		swapped;

	i = 0;
	while (env_array[i])
	{
		swapped = 0;
		j = 0;
		while (env_array[j + 1])
		{
			if (ft_strcmp(env_array[j], env_array[j + 1]) > 0)
			{
				temp = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = temp;
				swapped = 1;
			}
			j++;
		}
		if (!swapped)
			break ;
		i++;
	}
}

/*
Creates an array of strings from env_list for sorting
Returns: Array of strings or NULL if allocation fails
*/
static char	**create_env_array(t_env *env_list, int count_var)
{
	char	**env_array;
	char	*temp;
	int		i;

	i = 0;
	env_array = malloc(sizeof(char *) * (count_var + 1));
	if (!env_array)
		return (NULL);
	while (env_list)
	{
		temp = ft_strjoin("declare -x ", env_list->content);
		if (!temp)
		{
			free_array(env_array);
			return (NULL);
		}
		env_array[i++] = temp;
		env_list = env_list->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

/*
Display sorted environment variables with "declare -x" before each var
Returns: SUCCESS or ERR_MALLOC if allocation fails
*/
static t_error	print_sorted_env(t_env	*env_list)
{
	size_t	count_var;
	char	**env_array;
	int		i;

	count_var = count_env_var(env_list);
	env_array = create_env_array(env_list, count_var);
	if (!env_array)
		return (ERR_MALLOC);
	sort_env_array(env_array);
	i = 0;
	while (env_array[i])
	{
		ft_putendl_fd(env_array[i], 1);
		i++;
	}
	free_array(env_array);
	return (SUCCESS);
}

int	is_valid_var_name(char *arg)
{
	int	i;

	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (!ft_isalnum(arg[i]) && arg[0] != '_')
			return (0);
		i++;
	}
	return (1);
}

t_error	handle_export_assignement(char *arg, t_shell *s)
{
	char	*equal_sign;
	char	*key;
	t_env	*existing;
	t_error result;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		if(!find_env_var(s->env_list, arg) && is_valid_var_name(arg))
			return (add_envvar(&s->env_list, arg));
		if (find_env_var(s->env_list, arg))
			return (SUCCESS);
		return (ERR_EXPORT);
	}
	key = ft_strndup(arg, equal_sign - arg);
	if (!key)
		return (ERR_MALLOC);
	existing = find_env_var(s->env_list, key);
	if (existing)
	{
		free(existing->content);
		existing->content = ft_strdup(arg);
		if (!existing->content)
		{
			free(key);
			return (ERR_MALLOC);
		}
		free(existing->value);
		existing->value = ft_strdup(equal_sign + 1);
		if (!existing->value)
		{
			free(existing->content);
			free(key);
			return (ERR_MALLOC);
		}
		free(key);
		return (SUCCESS);
	}
	result = add_envvar(&s->env_list, arg);
	free(key);
	return (result);
}

t_error	execute_export(char **args, t_shell *s)
{
	int	count_args;
	int	i;

	count_args = 0;
	while (args[count_args])
		count_args++;
	if (count_args == 1)
	{
		s->exit_status = 0;
		return (print_sorted_env(s->env_list));
	}
	i = 1;
	while (args[i])
	{
		return(handle_export_assignement(args[i], s));
	}
	s->exit_status = 0;
	return (SUCCESS);
}
