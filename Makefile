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
		$(SRCS_DIR)/parcing/token_creation.c \
		$(SRCS_DIR)/parcing/token_extraction_utils.c \
		$(SRCS_DIR)/parcing/token_extraction.c \
		$(SRCS_DIR)/parcing/token_expansion.c \
		$(SRCS_DIR)/parcing/token_expansion_quotes.c \
		$(SRCS_DIR)/parcing/token_expansion_var.c \
		$(SRCS_DIR)/parcing/token_extraction_heredoc.c \
		$(SRCS_DIR)/parcing/token_extraction_heredoc_utils.c \
		$(SRCS_DIR)/execution/execute_command.c \
		$(SRCS_DIR)/execution/command_path.c \
		$(SRCS_DIR)/execution/execute_built_in.c \
		$(SRCS_DIR)/built-in/echo.c \
		$(SRCS_DIR)/built-in/pwd.c \
		$(SRCS_DIR)/env/init_env.c \
		$(SRCS_DIR)/init/init_data.c \
		$(SRCS_DIR)/utils/ft_strndup.c \
		$(SRCS_DIR)/utils/ft_strcmp.c \
		$(SRCS_DIR)/utils/ft_strcpy.c \
		$(SRCS_DIR)/utils/ft_strstr.c \
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
