/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 21:09:36 by feazeved          #+#    #+#             */
/*   Updated: 2025/06/30 23:46:05 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	main(int argc, char *argv[], char **envp)
{
	t_pipex	*pipex;
	int		exit_status;

	pipex = ft_init_pipex(argc, argv, envp);
	ft_exec(pipex);
	exit_status = ft_wait_children(pipex);
	ft_free(pipex);
	return (exit_status);
}

int	ft_wait_children(t_pipex *pipex)
{
	int	status;
	int	i;
	int	last_status;

	i = 0;
	last_status = 0;
	ft_close_pipes(pipex, 0);
	while (i < pipex->cmd_num)
	{
		waitpid(pipex->pids[i], &status, 0);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else
			last_status = 1;
		i++;
	}
	return (last_status);
}

void	ft_here_doc(t_pipex *pipex)
{
	int		pipe_fds[2];
	char	*line;

	if (pipe(pipe_fds) == -1)
		ft_error_exit(pipex, "pipe", strerror(errno), EXIT_FAILURE);
	write(1, "here_doc> ", 10);
	line = get_next_line(0, pipex);
	while (line)
	{
		if (!ft_strncmp(line, pipex->limiter, ft_strlen(pipex->limiter))
			&& line[ft_strlen(pipex->limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(pipe_fds[1], line, ft_strlen(line));
		free(line);
		write(1, "here_doc> ", 10);
		line = get_next_line(0, pipex);
	}
	pipex->infile = pipe_fds[0];
	close(pipe_fds[1]);
}

void	ft_exec(t_pipex *pipex)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < pipex->cmd_num)
	{
		pid = fork();
		if (pid == -1)
			ft_error_exit(pipex, "fork", strerror(errno), EXIT_FAILURE);
		if (pid == 0)
			ft_exec_child(pipex, i);
		pipex->pids[i] = pid;
		i++;
	}
}
