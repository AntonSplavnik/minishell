/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:38:17 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/13 18:27:28 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPES_H
# define PIPES_H

# include "minishell.h"

typedef struct s_pipe
{
	int		prev_pipe;
	int		next_pipe[2];
	pid_t	pid;
}	t_pipe;

typedef struct s_pipe_info
{
	int	prev_pipe;
	int	cmd_count;
	int	pipe_fd[2];
}	t_pipe_info;

//pipes_exec.c
t_error	create_pipe_and_fork(int cmd_count, int pipe_fd[2], pid_t *pid);
t_error	process_child(t_token *cmd, t_pipe_info *pinfo, t_shell *s);
void	set_output_fd(int cmd_count, int pipe_fd[2], int *out_fd);

//pipes_utils.c
t_error	process_parent(int *prev_pipe, int pipe_fd[2], int cmd_count);
t_error	process_pipe_stage(t_shell *s, t_token **current, \
					int *prev_pipe, int cmd_count);
int		count_pipes(t_token *tokens);
t_token	*get_next_cmd(t_token **tokens);

//pipes.c
int		has_pipe(t_shell *s);
t_error	handle_pipe_operations(t_shell *s);
t_error	execute_pipeline(t_shell *s, int cmd_count);
void	wait_for_children_and_set_exit_status(pid_t last_pid, t_shell *s);

#endif
