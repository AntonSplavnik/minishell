#Program name
NAME = minishell

#Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

#Add readline on Linux
CFLAGS += -I/usr/include/readline
READLINE = -lreadline

#Directories
SRCS_DIR = srcs
OBJS_DIR = objs
INCS_DIR = includes
LIBFT_DIR = libft

#Default target
all : $(NAME)

#Source files
SRCS = $(SRCS_DIR)/main.c \
		$(SRCS_DIR)/parsing/token_creation.c \
		$(SRCS_DIR)/parsing/token_extraction_utils.c \
		$(SRCS_DIR)/parsing/token_extraction.c \
		$(SRCS_DIR)/parsing/token_expansion.c \
		$(SRCS_DIR)/parsing/token_expansion_quotes.c \
		$(SRCS_DIR)/parsing/token_expansion_var.c \
		$(SRCS_DIR)/parsing/token_expansion_utils.c \
		$(SRCS_DIR)/parsing/token_extraction_heredoc.c \
		$(SRCS_DIR)/parsing/token_extraction_heredoc_utils.c \
		$(SRCS_DIR)/parsing/token_extraction_quotes.c	\
		$(SRCS_DIR)/parsing/token_extraction_export.c	\
		$(SRCS_DIR)/execution/command_path.c \
		$(SRCS_DIR)/execution/builtin/execute_built_in.c \
		$(SRCS_DIR)/execution/execute_child.c \
		$(SRCS_DIR)/execution/execute_command.c \
		$(SRCS_DIR)/execution/execute_prepare_cmd_args.c\
		$(SRCS_DIR)/execution/execute_single.c \
		$(SRCS_DIR)/execution/execute_utils.c \
		$(SRCS_DIR)/execution/fd_management.c \
		$(SRCS_DIR)/execution/pipes/pipes_exec.c \
		$(SRCS_DIR)/execution/pipes/pipes_utils.c \
		$(SRCS_DIR)/execution/pipes/pipes.c \
		$(SRCS_DIR)/execution/redirections/redirections.c \
		$(SRCS_DIR)/execution/redirections/redirections_utils.c \
		$(SRCS_DIR)/execution/redirections/heredoc.c \
		$(SRCS_DIR)/execution/signal_utils.c \
		$(SRCS_DIR)/execution/token_utils.c \
		$(SRCS_DIR)/built-in/echo.c \
		$(SRCS_DIR)/built-in/pwd.c \
		$(SRCS_DIR)/built-in/cd.c \
		$(SRCS_DIR)/built-in/env.c \
		$(SRCS_DIR)/built-in/exit.c \
		$(SRCS_DIR)/built-in/unset.c \
		$(SRCS_DIR)/built-in/export.c \
		$(SRCS_DIR)/built-in/export_handlers.c \
		$(SRCS_DIR)/built-in/export_array.c \
		$(SRCS_DIR)/built-in/export_format.c \
		$(SRCS_DIR)/env/init_env.c \
		$(SRCS_DIR)/env/manipulate_env.c \
		$(SRCS_DIR)/init/init_data.c \
		$(SRCS_DIR)/utils/ft_strndup.c \
		$(SRCS_DIR)/utils/ft_strcmp.c \
		$(SRCS_DIR)/utils/ft_strcpy.c \
		$(SRCS_DIR)/utils/ft_strstr.c \
		$(SRCS_DIR)/signals/signals_interactive.c \
		$(SRCS_DIR)/signals/signals_child_parent.c \
		$(SRCS_DIR)/free.c \
		$(SRCS_DIR)/errors.c \
		$(SRCS_DIR)/utils_to_print.c

#Object files
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

HEADERS = $(INCS_DIR)/minishell.h $(LIBFT_DIR)/libft.h

#Libft files
LIBFT = $(LIBFT_DIR)/libft.a

#Create obj directories
$(OBJS_DIR):
	@mkdir -p $(sort $(dir $(OBJS)))

# Compile objects
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADERS) | $(OBJS_DIR)
	$(CC) $(CFLAGS) -I$(INCS_DIR)  -c $< -o $@

# Make libft
$(LIBFT):
	@make -C $(LIBFT_DIR)

#Link program
$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)

# Clean object files
clean:
	@make -C $(LIBFT_DIR) clean
	@rm -rf $(OBJS_DIR)

# Clean everything
fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)

# Rebuild
re: fclean all

# These rules do not correspond to files
.PHONY: all clean fclean re
