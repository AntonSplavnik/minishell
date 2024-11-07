/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:03:20 by abillote          #+#    #+#             */
/*   Updated: 2024/11/07 16:53:07 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Required Standard Libraries */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h>
# include <dirent.h>
# include <termios.h>
# include <errno.h>
# include <limits.h>

/* Readline Libraries */
# include <readline/readline.h>
# include <readline/history.h>

/*Include libft*/
# include "../libft/libft.h"

// typedef struct s_command
// {
// 	char	**args;
// 	char	*function;
// 	char	**splitted_input;
// }			t_command;

typedef struct s_tokens
{
	char			*type;
	char			*content;
	// struct s_tokens	*next;
}			t_tockens;

#endif
