/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:13:32 by asplavni          #+#    #+#             */
/*   Updated: 2025/03/13 18:19:38 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_HEREDOC_H
# define REDIRECTIONS_HEREDOC_H

// Redirection types
typedef enum e_redir_type
{
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_IN,
	REDIR_HEREDOC
}	t_redir_type;

// heredoc.c
int				create_heredoc_file(void);
void			save_stdin(t_shell *s);
void			restore_stdin(t_shell *s);
t_error			read_heredoc_input(int fd, char *delim);

// redirections_utils.c
t_error			file_error(char *filename, t_shell *s);
t_error			syntax_error(char *msg, t_shell *s);
int				is_redirection(t_token_type type);
t_token			*remove_redirections(t_token *cmd);

// redirections.c
t_redir_type	get_redir_type(t_token_type type);
t_error			handle_output(t_token *redir, t_shell *s);
t_error			handle_input(t_token *redir, t_shell *s);
t_error			handle_redirections_(t_token *cmd, t_shell *s);

// fd_management.c
void			save_standard_fds(int fds[2]);
void			restore_standard_fds(int fds[2]);
t_error			handle_heredoc_(t_token *redir, t_shell *s);

#endif
