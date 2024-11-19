# tests/Makefile

NAME = test_tokenizer

# Paths
SRC_PATH = ../srcs
INC_PATH = ../includes
LIBFT_PATH = ../libft

# Source files
SRC = 	$(SRC_PATH)/parcing/tokenisation.c \
		$(SRC_PATH)/parcing/parcing_utils.c \
		$(SRC_PATH)/env/init_env.c \
		$(SRC_PATH)/utils/ft_strndup.c \
		$(SRC_PATH)/utils/ft_strcmp.c \
		$(SRC_PATH)/free.c \
		$(SRC_PATH)/errors.c \
		$(SRC_PATH)/utils_to_print.c \
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
