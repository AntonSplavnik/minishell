/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 10:03:42 by abillote          #+#    #+#             */
/*   Updated: 2025/02/12 10:55:56 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Counts total number of environment variables in linked list
- Iterates through entire environment list once
- Used to allocate proper array size for sorting
- Handles empty list case
Parameters:
 - env_list: Head of environment variable linked list
Returns: Total count of environment variables as size_t
*/
size_t	count_env_var(t_env *env_list)
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
Sorts array of environment strings using bubble sort algorithm
- Performs in-place sorting of strings in alphabetical order
- Uses string comparison to determine ordering
- Continues until no more swaps are needed
- Handles array termination with NULL
Parameters:
 - env_array: NULL-terminated array of environment strings to sort
*/
void	sort_env_array(char **env_array)
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
Stores a single environment variable in array with proper formatting
- Handles both variables with and without values
- For variables with values: formats as "declare -x KEY="VALUE""
- For variables without values: formats as "declare -x KEY"
- Properly frees array on any error
Parameters:
 - current: Current environment variable to store
 - env_array: Target array for storage
 - i: Index in array to store at
Returns: 1 on success, 0 on any failure
*/
int	store_env_var_in_array(t_env *current, char **env_array, int i)
{
	if (ft_strchr(current->content, '='))
	{
		env_array[i] = format_env_with_equals(current, env_array);
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
Creates array of formatted environment strings for sorting
- Allocates array based on provided count plus NULL terminator
- Converts linked list format to array format
- Formats each variable with proper declare -x and quotes syntax
- Properly frees partial array on any error
Parameters:
 - env_list: Environment list to convert
 - count_var: Pre-calculated number of variables
Returns: Allocated and populated array or NULL on error
*/
char	**create_env_array(t_env *env_list, int count_var)
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
