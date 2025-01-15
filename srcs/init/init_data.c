/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:47:25 by abillote          #+#    #+#             */
/*   Updated: 2025/01/15 11:04:33 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_error	initialize_shell(t_shell **s, char **env)
{
	t_error	error;

	*s = malloc(sizeof(t_shell));
	if (!*s)
		return (ERR_MALLOC);
	(*s)->token_list = NULL;
	(*s)->env_list = NULL;
	(*s)->exit_status = 0;
	error = init_env(&(*s)->env_list, env);
	if (error != SUCCESS)
	{
		free(*s);
		*s = NULL;
		return (error);
	}
	return (SUCCESS);
}
