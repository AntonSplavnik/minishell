/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:13:23 by abillote          #+#    #+#             */
/*   Updated: 2025/03/18 15:58:59 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_count_args(char **args)
{
	int	count_args;

	count_args = 0;
	while (args[count_args])
		count_args++;
	return (count_args);
}

static int	has_overflow(size_t len, int sign, char *s)
{
	if (len > 19 || (len == 19 && sign == 1 \
		&& ft_strcmp(s + (s[0] == '+' || s[0] == '-'), \
		"9223372036854775807") > 0) || (len == 19 && sign == -1 && \
		ft_strcmp(s + 1, "9223372036854775808") > 0))
		return (1);
	return (0);
}

/*
Checks if the string represents a valid exit code:
- Must contain only digits (with optional leading sign)
- Must be within the range of a long long int
- No overflow/underflow
*/
static int	is_valid_exit_code(char *s)
{
	int		i;
	int		sign;
	size_t	len;

	i = 0;
	sign = 1;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	if (!s[i])
		return (0);
	len = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		len++;
		i++;
	}
	if (has_overflow(len, sign, s))
		return (0);
	return (1);
}

void	simple_exit(t_shell *s, char **args)
{
	int	exit_code;

	exit_code = s->exit_status;
	free_all(s, args);
	exit(exit_code);
}

t_error	execute_exit(char **args, t_shell *s)
{
	int	count_args;
	int	exit_code;

	count_args = ft_count_args(args);
	restore_standard_fds(s);
	if (count_args == 1)
		simple_exit(s, args);
	if (!is_valid_exit_code(args[1]))
	{
		print_error_builtin("exit", args[1], ": numeric argument required");
		free_all(s, args);
		exit(2);
	}
	if (count_args > 2)
	{
		s->exit_status = 1;
		return (ERR_EXIT_ARGS);
	}
	exit_code = ft_atoi(args[1]);
	free_all(s, args);
	exit(exit_code % 256);
	return (SUCCESS);
}
