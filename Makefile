# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/09 23:34:34 by feazeved          #+#    #+#              #
#    Updated: 2025/06/09 23:40:50 by feazeved         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

NAME = pipex

CFLAGS = -Wall -Wextra -Werror

SRC = ft_pipex_utils.c ft_split.c pipex.c

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

re: fclean all
