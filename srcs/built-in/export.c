/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:13:03 by abillote          #+#    #+#             */
/*   Updated: 2025/02/07 18:13:28 by abillote         ###   ########.fr       */
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
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
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
*/

int	store_env_var_in_array(t_env *current, char **env_array, int i)
{
	char	*temp1;
	char	*temp2;

	if (ft_strchr(current->content, '='))
	{
		temp1 = ft_strjoin("declare -x ", current->key);
		if (!temp1)
		{
			free_array(env_array);
			return (0);
		}
		temp2 = ft_strjoin(temp1, "=\"");
		free(temp1);
		if (!temp2)
		{
			free_array(env_array);
			return (0);
		}
		temp1 = ft_strjoin(temp2, current->value);
		free(temp2);
		if (!temp1)
		{
			free_array(env_array);
			return (0);
		}
		env_array[i] = ft_strjoin(temp1, "\"");
		free(temp1);
	}
	else
		env_array[i] = ft_strjoin("declare -x ", current->content);
	if (!env_array[i])
	{
		free_array(env_array);
		return (0);
	}
	return (1);
}

/*
Creates an array of strings from env_list for sorting
Returns: Array of strings or NULL if allocation fails
*/
static char	**create_env_array(t_env *env_list, int count_var)
{
	char	**env_array;
	t_env	*current;
	int		i;

	i = 0;
	env_array = malloc(sizeof(char *) * (count_var + 1));
	if (!env_array)
		return (NULL);
	current = env_list;
	while (current)
	{
		if (!store_env_var_in_array(current, env_array, i))
			return (NULL);
		i++;
		current = current->next;
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

	if (!env_list)
		return (SUCCESS);
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
	char	*append_sign;
	char	*key;
	t_env	*existing;
	t_error result;

	equal_sign = ft_strchr(arg, '=');
	append_sign = ft_strstr(arg, "+=");
	if (arg[0] == '=')
		return (ERR_EXPORT);
	if (!equal_sign)
	{
		if(!find_env_var(s->env_list, arg) && is_valid_var_name(arg))
			return (add_envvar(&s->env_list, arg));
		if (find_env_var(s->env_list, arg))
			return (SUCCESS);
		return (ERR_EXPORT);
	}
	if (append_sign)
	{
		key = ft_strndup(arg, append_sign - arg);
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
			char *temp = ft_strjoin(existing->value, ft_strdup(append_sign + 2));
			if (!temp)
			{
				free(existing->content);
				free(key);
				return (ERR_MALLOC);
			}
			free(existing->value);
			existing->value = ft_strdup(temp);
			free(temp);
			if (!existing->value)
			{
				free(existing->content);
				free(key);
				return (ERR_MALLOC);
			}
			free(key);
			return (SUCCESS);
		}
		if (is_valid_var_name(key))
			result = add_envvar(&s->env_list, arg);
		else
			result = ERR_EXPORT;
		free(key);
		return (result);
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
	if (is_valid_var_name(key))
		result = add_envvar(&s->env_list, arg);
	else
		result = ERR_EXPORT;
	free(key);
	return (result);
}

//static char *combine_quoted_tokens(char **args, int *i)
//{
//	// Skip empty tokens
//	if (args[*i] && ft_strcmp(args[*i], "") == 0)
//	{
//		return (ft_strdup(""));
//	}

//	// For tokens without equals sign, just return them
//	if (!ft_strchr(args[*i], '='))
//		return (ft_strdup(args[*i]));

//	// Handle tokens with equals sign and combining what follows
//	char	*result = ft_strdup(args[*i]);
//	if (!result)
//		return (NULL);

//	(*i)++;
//	while (args[*i] && !ft_strchr(args[*i], '='))
//	{
//		printf("arg i is %s\n", args[*i]);
//		char *temp = result;
//		result = ft_strjoin(temp, args[*i]);
//		free(temp);
//		if (!result)
//			return (NULL);
//		(*i)++;
//	}
//	(*i)--; // Step back one as the main loop will increment
//	return (result);
//}

t_error	execute_export(char **args, t_shell *s)
{
	int		count_args;
	int		i;
	t_error	result;
	//char	*combined_tokens;

	count_args = 0;
	while (args[count_args])
		count_args++;
	if (count_args == 1 || (count_args == 2 && ft_strcmp(args[1], "") == 0))
	{
		s->exit_status = 0;
		return (print_sorted_env(s->env_list));
	}
	i = 1;
	while (args[i])
	{
		//printf("args[i] is %s\n", args[i]);
		//combined_tokens = combine_quoted_tokens(args, &i);
		//if (!combined_tokens)
		//	return (ERR_MALLOC);
		//printf("the combined token is %s\n", combined_tokens);
		//if (ft_strcmp(combined_tokens, "") != 0)
		result = handle_export_assignement(args[i], s);
		if (result != SUCCESS)
		{
			print_error(result);
			s->exit_status = 1;
		}
		//free(combined_tokens);
		//printf("is is %d\n", i);
		i++;
	}
	s->exit_status = 0;
	return (SUCCESS);
}
