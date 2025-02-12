/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_format.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 10:06:50 by abillote          #+#    #+#             */
/*   Updated: 2025/02/12 10:36:38 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Creates prefix portion of formatted environment variable
- Combines "declare -x " with variable key
- Adds =" for variables with values
- Properly handles memory allocation failures
- Frees array on any error
Parameters:
 - current: Environment variable being formatted
 - env_array: Array to free on error (used only in case of errors)
Returns: Formatted prefix string or NULL on error
*/
char	*format_env_prefix(t_env *current, char **env_array)
{
	char	*temp1;
	char	*temp2;

	temp1 = ft_strjoin("declare -x ", current->key);
	if (!temp1)
	{
		free_array(env_array);
		return (NULL);
	}
	temp2 = ft_strjoin(temp1, "=\"");
	free(temp1);
	if (!temp2)
	{
		free_array(env_array);
		return (NULL);
	}
	return (temp2);
}

/*
Formats environment variable with value for display
- Uses format_env_prefix for initial formatting
- Adds variable value after prefix
- Adds closing quote
- Handles memory management for temporary strings
- Properly frees resources on any error
Parameters:
 - current: Environment variable to format
 - env_array: Array to free on error (used only in case of errors)
Returns: Fully formatted string or NULL on error
*/
char	*format_env_with_equals(t_env *current, char **env_array)
{
	char	*temp1;
	char	*temp2;
	char	*result;

	temp2 = format_env_prefix(current, env_array);
	if (!temp2)
		return (NULL);
	temp1 = ft_strjoin(temp2, current->value);
	free(temp2);
	if (!temp1)
	{
		free_array(env_array);
		return (NULL);
	}
	result = ft_strjoin(temp1, "\"");
	free(temp1);
	return (result);
}
