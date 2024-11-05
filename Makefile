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

#Source files
SRCS = $(SRCS_DIR)/main.c \

#Object files
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

#Libft files
LIBFT = $(LIBFT_DIR)/libft.a

#Default target
all : $(NAME)

#Create obj directories
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# Compile objects
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@$(CC) $(CFLAGS) -I$(INCS_DIR)  -c $< -o $@

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
