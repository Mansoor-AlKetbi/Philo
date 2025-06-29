# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mal-ketb <mal-ketb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/01 12:49:01 by mal-ketb          #+#    #+#              #
#    Updated: 2025/06/01 12:49:04 by mal-ketb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = philo

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -pthread -I.

SRCS    = $(wildcard *.c)
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all