# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mal-ketb <mal-ketb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/01 12:49:01 by mal-ketb          #+#    #+#              #
#    Updated: 2025/07/13 12:08:42 by mal-ketb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = philo

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror
THREAD  = -pthread

SRCS    = main.c parsing.c print.c threads.c utils.c
OBJS    = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(THREAD)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re