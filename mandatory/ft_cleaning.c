/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cleaning.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 00:46:04 by feazeved          #+#    #+#             */
/*   Updated: 2025/06/29 20:48:40 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_error_exit(t_pipex *pipex, char *func, char *msg, int status)
{
	if (pipex)
		ft_free(pipex);
	if (func)
	{
		write(2, func, ft_strlen(func));
		write(2, ": ", 2);
	}
	if (pipex && pipex->pipes)
		ft_close_pipes(pipex, 0);
	if (msg)
		write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(status);
}

void	ft_child_error_exit(char *func, char *msg, char **to_free, int status)
{
	if (func)
	{
		write(2, func, ft_strlen(func));
		write(2, ": ", 2);
	}
	if (msg)
		write(2, msg, ft_strlen(msg));
	if (msg || func)
		write(2, "\n", 1);
	if (to_free)
		ft_free_strs(to_free);
	exit(status);
}

void	ft_free(t_pipex *pipex)
{
	int	i;

	if (!pipex)
		return ;
	if (pipex->outfile != -1)
		close(pipex->outfile);
	if (pipex->infile != -1)
		close(pipex->infile);
	if (pipex->pipes)
	{
		i = 0;
		while (i < pipex->cmd_num - 1)
		{
			if (pipex->pipes[i])
				free(pipex->pipes[i]);
			i++;
		}
		free(pipex->pipes);
	}
	if (pipex->pids)
		free(pipex->pids);
	free(pipex);
}

void	ft_free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

void	ft_close_pipes(t_pipex *pipex, int limiter)
{
	int	i;

	i = 0;
	if (!limiter)
	{
		while (i < pipex->cmd_num - 1)
		{
			close(pipex->pipes[i][0]);
			close(pipex->pipes[i][1]);
			i++;
		}
	}
	else
	{
		i = limiter;
		while (--i >= 0)
		{
			close(pipex->pipes[i][0]);
			close(pipex->pipes[i][1]);
		}
	}
}
