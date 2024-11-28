# tests/Makefile

NAME = test_tokenizer

# Paths
SRCS_DIR = ../srcs
INC_PATH = ../includes
LIBFT_PATH = ../libft

# Source files
SRC = 	$(SRCS_DIR)/parcing/token_creation.c \
		$(SRCS_DIR)/parcing/token_utils.c \
		$(SRCS_DIR)/parcing/token_extraction.c \
		$(SRCS_DIR)/parcing/token_expansion.c \
		$(SRCS_DIR)/env/init_env.c \
		$(SRCS_DIR)/utils/ft_strndup.c \
		$(SRCS_DIR)/utils/ft_strcmp.c \
		$(SRCS_DIR)/utils/ft_strcpy.c \
		$(SRCS_DIR)/free.c \
		$(SRCS_DIR)/errors.c \
		$(SRCS_DIR)/utils_to_print.c \
		tests_tokenizer.c \

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INC_PATH)

# Library flags
LIBS = -L$(LIBFT_PATH) -lft

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
