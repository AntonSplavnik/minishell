/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:58:40 by abillote          #+#    #+#             */
/*   Updated: 2025/03/14 12:18:14 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# define MAX_HEREDOCS 16

# include "minishell.h"

//Used during token extraction to store
//quote information and pass it to different functions
typedef struct s_quote_info
{
	char	quote_char;
	int		num_quote;
}			t_quote_info;

//Used during token extraction to store heredoc info
typedef struct s_heredoc_info
{
	char	*delim;
	int		is_quoted;
}	t_heredoc_info;

//Used during token expansion to pass more than 4 args
typedef struct s_parse_params
{
	size_t	i;
	size_t	j;
	int		in_squote;
	int		in_dquote;
}	t_parse_params;

//parsing
//token_creation.c
int				is_command(t_token **token_list);
t_token_type	get_token_type(char *input, t_token **token_list);
t_token			*create_token(char *input, t_token_type type);
t_error			add_token(t_token **token_list, char *input, t_token_type type);
t_error			input_to_token(t_token **token_list, char *args);

//token_extraction.c
char			*extract_unquoted_token(const char *str, \
					size_t *i, t_error *error);
char			*extract_content_without_quotes(char *token, size_t *len);
char			*extract_quoted_token(char *args, size_t *i, t_error *error);
t_error			ft_split_token(t_token **token_list, \
					char *args, size_t *i, char **token);

//token_expansion.c
size_t			calculate_length_loop(t_token *token, t_shell *s, \
					size_t *expanded_len);
size_t			calculate_expanded_length(t_token *token, t_shell *s);
void			fill_token_content_after_expansion(t_token *t, \
					char *exp, t_shell *s);
t_error			expand_tokens(t_shell *s);

//token_extraction_utils.c
int				is_space(char c);
int				is_delimiter(char c);
int				get_delimiter_len(const char *str);
t_token			*get_last_token(t_token *token_list);
t_token_type	get_other_types(char *input);

//token_expansion_var.c
char			*get_var_value(t_env *env_list, char *key, \
					int status, int *need_free);
char			*get_var_name(const char *str);
void			get_var_length(const char *str, t_shell *s, \
						size_t *i, size_t *expanded_len);
void			expand_var(const char *str, t_shell *s, \
						size_t *i, char *expanded);

//token_expansion_quotes.c
void			init_expansion_params(size_t *i, size_t *j, \
						int *in_squote, int *in_dquote);
void			process_quote(char quote_char, int *in_squote, \
						int *in_dquote);

//token_extraction_heredoc
t_error			handle_heredoc(t_token **token_list, \
						char *delimiter);
t_error			process_heredocs(t_token **token_list);
char			*handle_line_input(char *content, char *line);

//token_extraction_heredoc_utils
int				find_heredoc_delimiters(t_token *token_list, \
					t_token *delim_tokens[MAX_HEREDOCS]);
char			*handle_heredoc_cleanup(char *content, char *line);
t_heredoc_info	*get_heredoc_info(char *delim);
int				*get_heredoc_state_ptr(void);
void			reset_heredoc_state(void);

//token_extraction_quotes
char			*extract_double_quotes(char *args, size_t *i, \
						t_error *error, char quote_char);
char			*extract_quoted_token_helper(char *args, \
					size_t *i, t_error *error);
size_t			handle_quoted_content(char *args, size_t *i, \
					t_quote_info *quotes);

//token_extraction_export
char			*extract_export_token(char *args, size_t *i, \
						t_error *error);

//token_expansion_utils
void			process_dollar_length(t_token *token, t_shell *s,
					size_t *i, size_t *expanded_len);
void			process_dollar_expansion(t_token *t, char *exp,
					t_shell *s, t_parse_params *pars);

//token_extraction_string
void			free_if_exist(void *ptr1, void *ptr2);
char			*concatenate_tokens(char *token1, char *token2);
char			*process_more_adjacent_quotes(char *args, \
					size_t *i, char *combined, t_error *error);
char			*handle_adjacent_quotes(char *args, size_t *i, \
					char *token, t_error *error);
char			*handle_dollar_after_quote(char *args, \
					size_t *i, char *token, t_error *error);

#endif
