/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:13:03 by abillote          #+#    #+#             */
/*   Updated: 2025/02/12 15:09:36 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Displays environment variables in sorted order with proper formatting
- Creates array of environment variables for sorting
- Sorts array alphabetically before displaying
- Each variable is displayed with "declare -x" prefix
- Values are enclosed in quotes if variable contains an equals sign
- Properly frees allocated memory after display
Returns:
- SUCCESS if environment list is empty or display succeeds
- ERR_MALLOC if memory allocation fails during array creation
*/
t_error	print_sorted_env(t_env	*env_list)
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

/*
Validates if a string is a valid environment variable name
- First character must be alphabetic or underscore
- Subsequent characters must be alphanumeric or underscore
- Returns 1 if name is valid, 0 otherwise
- Used for both export and assignment validation
Parameters:
- arg: String to validate as variable name
*/
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

/*
Main export assignment handler that calls the appropriate sub-handler
- Handles three cases: normal assignment (=), append (+=), and no assignment
- Detects assignment type by searching for = or += in argument
- Validates initial character is not an equals sign
- Call specialized handlers based on assignment type
Parameters:
- arg: Export argument to process
- s: Shell structure containing environment list
Returns: SUCCESS or appropriate error code
*/
t_error	handle_export_assignement(char *arg, t_shell *s)
{
	char	*equal_sign;
	char	*append_sign;

	equal_sign = ft_strchr(arg, '=');
	append_sign = ft_strstr(arg, "+=");
	if (arg[0] == '=')
		return (ERR_EXPORT);
	if (!equal_sign)
		return (handle_no_equal_sign(arg, s));
	if (append_sign)
		return (handle_append_case(arg, append_sign, s));
	return (handle_equal_case(arg, equal_sign, s));
}

/*
Helper function to process export arguments
- Processes each argument through handle_export_assignement
- Keeps track of number of errors encountered
- Returns number of errors that occurred
Parameters:
- args: Array of command arguments
- s: Shell structure
*/
static int	process_export_args(char **args, t_shell *s)
{
	int		i;
	int		nb_errors;
	t_error	result;

	i = 1;
	nb_errors = 0;
	while (args[i])
	{
		result = handle_export_assignement(args[i], s);
		if (result != SUCCESS)
		{
			print_error_builtin("export", args[i], ": not a valid identifier");
			nb_errors += 1;
		}
		i++;
	}
	return (nb_errors);
}

/*
Main export builtin command implementation
- With no args: displays sorted environment
- With args: processes each as an assignment and/or a variable creation
- Continue processing the args even in case of errors
- Sets exit status based on success of operations
Parameters:
- args: Command arguments array
- s: Shell structure
Returns: SUCCESS or error if env does not display
*/
t_error	execute_export(char **args, t_shell *s)
{
	int		count_args;
	int		nb_errors;

	count_args = 0;
	while (args[count_args])
		count_args++;
	if (count_args == 1 || (count_args == 2 && ft_strcmp(args[1], "") == 0))
	{
		s->exit_status = 0;
		return (print_sorted_env(s->env_list));
	}
	nb_errors = process_export_args(args, s);
	if (nb_errors > 0)
		s->exit_status = 1;
	else
		s->exit_status = 0;
	return (SUCCESS);
}
