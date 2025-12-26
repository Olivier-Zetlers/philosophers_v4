# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::   #
#    Makefile                                           :+:      :+:    :+:   #
#                                                     +:+ +:+         +:+     #
#    By: ozetlers <marvin@42.fr>                    +#+  +:+       +#+        #
#                                                 +#+#+#+#+#+   +#+           #
#    Created: 2025/12/26 03:40:00 by ozetlers          #+#    #+#             #
#    Updated: 2025/12/26 03:40:00 by ozetlers         ###   ########.fr       #
#                                                                              #
# **************************************************************************** #

NAME		= philo

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
INCLUDES	= -I./includes

SRCS_DIR	= srcs
SRCS		= $(SRCS_DIR)/main.c \
			  $(SRCS_DIR)/dinner.c \
			  $(SRCS_DIR)/init.c \
			  $(SRCS_DIR)/monitor.c \
			  $(SRCS_DIR)/parsing.c \
			  $(SRCS_DIR)/safe_wrappers.c \
			  $(SRCS_DIR)/sync.c \
			  $(SRCS_DIR)/sync_access.c \
			  $(SRCS_DIR)/time.c

OBJS		= $(SRCS:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:		%.c
			$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
			rm -f $(OBJS)

fclean:		clean
			rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
