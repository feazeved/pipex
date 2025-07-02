# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/09 23:34:34 by feazeved          #+#    #+#              #
#    Updated: 2025/07/02 20:11:20 by feazeved         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

NAME = pipex

CFLAGS = -Wall -Wextra -Werror

SRC = mandatory/ft_pipex_utils.c mandatory/ft_child.c mandatory/ft_cleaning.c mandatory/ft_parsing.c mandatory/ft_pipex_unique_utils.c  mandatory/ft_split.c mandatory/pipex.c mandatory/ft_gnl.c


SRC_BONUS = bonus/ft_pipex_utils.c bonus/ft_child.c bonus/ft_cleaning.c bonus/ft_parsing.c bonus/ft_pipex_unique_utils.c  bonus/ft_split.c bonus/pipex_bonus.c bonus/ft_gnl.c

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
