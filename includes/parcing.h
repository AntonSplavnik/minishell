/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:58:40 by abillote          #+#    #+#             */
/*   Updated: 2024/12/06 16:11:41 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARCING_H
# define PARCING_H

# include "types.h"

//Used during token extraction to store
//quote information and pass it to different functions
typedef struct s_quote_info
{
	int		in_inner_quote;
	char	inner_quote;
	char	outer_quote;
	int		num_outer_quote;
}			t_quote_info;

//Used during token expansion to pass more than 4 args
typedef struct s_parse_params {
	size_t	i;
	size_t	j;
	int		in_squote;
	int		in_dquote;
}	t_parse_params;

//PARCING
//token_creation.c
t_error				input_to_token(t_token **token_list, char *args);
t_token				*create_token(char *input, t_token_type type);
t_error				add_token(t_token **token_list, \
					char *input, t_token_type type);
t_token_type		get_token_type(char *input, t_token **token_list);
int					is_command(t_token **token_list);

//token_extraction.c
t_error				ft_split_token(t_token **token_list, \
						char *args, size_t *i, char **token);

//token_expansion.c
t_error				expand_tokens(t_token **token_list, t_env *env_list);

//token_extraction_utils.c
int					is_space(char c);
int					is_delimiter(char c);
int					get_delimiter_len(const char *str);

//token_expansion_var.c
char				*get_var_value(t_env *env_list, char *key);
char				*get_var_name(const char *str);
void				get_var_length(const char *str, \
						t_env *env_list, size_t *i, size_t *expanded_len);
void				expand_var(const char *str, \
						t_env *env_list, size_t *i, char *expanded);

//token_expansion_quotes.c
void				init_expansion_params(size_t *i, size_t *j, \
						int *in_squote, int *in_dquote);
void				process_quote(char quote_char, int *in_squote, \
						int *in_dquote);

#endif