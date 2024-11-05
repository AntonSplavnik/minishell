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

#Source files
SRCS = $(SRCS_DIR)/main.c \

#Object files
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

#Default target
all : $(NAME)

#Create obj directories
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# Compile objects
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@$(CC) $(CFLAGS) -I$(INCS_DIR)  -c $< -o $@

#Link program
$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(READLINE) -o $(NAME)

# Clean object files
clean:
	@rm -rf $(OBJS_DIR)

# Clean everything
fclean: clean
	@rm -f $(NAME)

# Rebuild
re: fclean all

# These rules do not correspond to files
.PHONY: all clean fclean re
