# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: moulmoud <moulmoud@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/03 16:48:46 by moulmoud          #+#    #+#              #
#    Updated: 2023/04/03 16:48:46 by moulmoud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc

#CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = $(wildcard *.c)

OBJS = $(SRCS:.c=.o)


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
