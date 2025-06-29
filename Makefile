# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/09 23:34:34 by feazeved          #+#    #+#              #
#    Updated: 2025/06/29 16:04:56 by feazeved         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc -g

NAME = pipex

CFLAGS = -Wall -Wextra -Werror

SRC = ft_pipex_utils.c ft_child.c ft_cleaning.c ft_parsing.c ft_pipex_unique_utils.c  ft_split.c pipex_bonus.c ft_gnl.c


SRC_BONUS = ft_pipex_utils.c ft_child.c ft_cleaning.c ft_parsing.c ft_pipex_unique_utils.c  ft_split.c bonus/pipex_bonus.c bonus/ft_gnl.c

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

bonus: clean
	$(CC) $(CFLAGS) $(SRC_BONUS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

re: fclean all
