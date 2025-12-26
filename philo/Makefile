NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I includes

SRC_DIR = srcs
OBJ_DIR = obj

SRC =	main.c \
		dinner.c \
		monitor.c \
		init.c \
		parsing.c \
		time.c \
		sync.c \
		safe_wrappers.c \
		sync_access.c

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -pthread $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c includes/philo.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
