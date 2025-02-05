# tests/Makefile

NAME = test_tokenizer

# Paths
SRCS_DIR = ../srcs
INC_PATH = ../includes
LIBFT_PATH = ../libft

# Source files
SRC = 	$(SRCS_DIR)/parsing/token_creation.c \
		$(SRCS_DIR)/parsing/token_creation.c \
		$(SRCS_DIR)/parsing/token_extraction_utils.c \
		$(SRCS_DIR)/parsing/token_extraction.c \
		$(SRCS_DIR)/parsing/token_expansion.c \
		$(SRCS_DIR)/parsing/token_expansion_quotes.c \
		$(SRCS_DIR)/parsing/token_expansion_var.c \
		$(SRCS_DIR)/parsing/token_extraction_heredoc.c \
		$(SRCS_DIR)/parsing/token_extraction_heredoc_utils.c \
		$(SRCS_DIR)/parsing/token_extraction_quotes.c	\
		$(SRCS_DIR)/execution/execute_command.c \
		$(SRCS_DIR)/execution/command_path.c \
		$(SRCS_DIR)/execution/execute_built_in.c \
		$(SRCS_DIR)/built-in/echo.c \
		$(SRCS_DIR)/built-in/pwd.c \
		$(SRCS_DIR)/built-in/cd.c \
		$(SRCS_DIR)/built-in/exit.c \
		$(SRCS_DIR)/built-in/unset.c \
		$(SRCS_DIR)/built-in/export.c \
		$(SRCS_DIR)/env/init_env.c \
		$(SRCS_DIR)/env/manipulate_env.c \
		$(SRCS_DIR)/init/init_data.c \
		$(SRCS_DIR)/utils/ft_strndup.c \
		$(SRCS_DIR)/utils/ft_strcmp.c \
		$(SRCS_DIR)/utils/ft_strcpy.c \
		$(SRCS_DIR)/utils/ft_strstr.c \
		$(SRCS_DIR)/free.c \
		$(SRCS_DIR)/errors.c \
		$(SRCS_DIR)/utils_to_print.c
		tests_tokenizer.c \

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIE -I$(INC_PATH)

# Library flags
LIBS = -L$(LIBFT_PATH) -lft -lreadline

all: $(NAME)
	@./$(NAME)

$(NAME): libft $(SRC)
	@$(CC) $(CFLAGS) $(SRC) $(LIBS) -o $(NAME)

libft:
	@make -C $(LIBFT_PATH)

clean:
	@rm -f $(NAME)
	@make -C $(LIBFT_PATH) clean

fclean: clean
	@make -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re libft
