/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 21:09:36 by feazeved          #+#    #+#             */
/*   Updated: 2025/07/02 20:04:11 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
