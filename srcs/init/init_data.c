/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:47:25 by abillote          #+#    #+#             */
/*   Updated: 2025/03/17 14:16:18 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Initializes the shell structure:
- Allocates memory for shell structure
- Initializes internal state variables
- Sets up environment variable list
- Handles initialization errors
Returns: SUCCESS or ERR_MALLOC if allocation fails
*/
t_error	initialize_shell(t_shell **s, char **env)
{
	t_error	error;

	*s = malloc(sizeof(t_shell));
	if (!*s)
		return (ERR_MALLOC);
	(*s)->token_list = NULL;
	(*s)->env_list = NULL;
	(*s)->exit_status = 0;
	(*s)->stdin_copy = -1;
	(*s)->last_pid = 0;
	error = init_env(*s, &(*s)->env_list, env);
	if (error != SUCCESS)
	{
		free(*s);
		*s = NULL;
		return (error);
	}
	return (SUCCESS);
}
