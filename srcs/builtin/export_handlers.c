/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 09:59:58 by abillote          #+#    #+#             */
/*   Updated: 2025/02/12 10:55:37 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Handles export of variable without assignment
- Validates variable name format
- Checks if variable already exists
- Adds new variable if name valid and doesn't exist
- Simply succeeds if variable already exists
Parameters:
 - arg: Variable name to process
 - s: Shell structure containing environment list
Returns: SUCCESS, ERR_EXPORT if invalid name, or ERR_MALLOC
*/
t_error	handle_no_equal_sign(char *arg, t_shell *s)
{
	if (!find_env_var(s->env_list, arg) && is_valid_var_name(arg))
		return (add_envvar(&s->env_list, arg));
	if (find_env_var(s->env_list, arg))
		return (SUCCESS);
	return (ERR_EXPORT);
}

/*
Updates values of existing environment variable
- Frees old content and value
- Creates new content string with key and updated value
- Handles memory allocation failures
- Maintains proper memory state on errors
Parameters:
 - existing: Variable to update
 - new_value: New processed value string
Returns: SUCCESS or ERR_MALLOC
*/
t_error	update_existing_var(t_env *existing, char *new_value)
{
	char	*temp;

	free(existing->content);
	temp = ft_strjoin(existing->key, "=");
	if (!temp)
		return (ERR_MALLOC);
	existing->content = ft_strjoin(temp, new_value);
	if (!existing->content)
		return (ERR_MALLOC);
	free(temp);
	free(existing->value);
	existing->value = ft_strdup(new_value);
	if (!existing->value)
	{
		free(existing->content);
		return (ERR_MALLOC);
	}
	return (SUCCESS);
}

/*
Processes append operation for environment variable
- Concatenates existing value with new value
- Updates variable with combined value
- Handles memory management for temporary strings
- Properly cleans up on any error
Parameters:
 - append_sign: Pointer to += in argument
 - existing: Variable to append to
Returns: SUCCESS or error code
*/
t_error	handle_append_operation(char *append_sign, t_env *existing)
{
	char	*temp;
	t_error	result;

	temp = ft_strjoin(existing->value, append_sign + 2);
	if (!temp)
	{
		free(existing->content);
		return (ERR_MALLOC);
	}
	result = update_existing_var(existing, temp);
	free(temp);
	return (result);
}

/*
Handles += append operation for variables
- Extracts variable name from argument
- Handles both existing and new variables
- Performs append for existing variables
- Creates new variable if doesn't exist
Parameters:
 - arg: Full argument string
 - append_sign: Pointer to += in argument
 - s: Shell structure containing environment list
Returns: SUCCESS or appropriate error code
*/
t_error	handle_append_case(char *arg, char *append_sign, t_shell *s)
{
	char	*key;
	t_env	*existing;
	t_error	result;

	key = ft_strndup(arg, append_sign - arg);
	if (!key)
		return (ERR_MALLOC);
	existing = find_env_var(s->env_list, key);
	if (existing)
	{
		result = handle_append_operation(append_sign, existing);
		free(key);
		return (result);
	}
	if (is_valid_var_name(key))
		result = add_envvar(&s->env_list, arg);
	else
		result = ERR_EXPORT;
	free(key);
	return (result);
}

/*
Handles standard = assignment operation
- Extracts variable name from argument
- Updates existing variable if found
- Creates new variable if doesn't exist
- Validates variable name format
Parameters:
 - arg: Full argument string
 - equal_sign: Pointer to = in argument
 - s: Shell structure containing environment list
Returns: SUCCESS or appropriate error code
*/
t_error	handle_equal_case(char *arg, char *equal_sign, t_shell *s)
{
	char	*key;
	t_env	*existing;
	t_error	result;

	key = ft_strndup(arg, equal_sign - arg);
	if (!key)
		return (ERR_MALLOC);
	existing = find_env_var(s->env_list, key);
	if (existing)
	{
		result = update_existing_var(existing, equal_sign + 1);
		free(key);
		return (result);
	}
	if (is_valid_var_name(key))
		result = add_envvar(&s->env_list, arg);
	else
		result = ERR_EXPORT;
	free(key);
	return (result);
}
