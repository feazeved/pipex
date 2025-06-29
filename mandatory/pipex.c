/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 19:15:12 by feazeved          #+#    #+#             */
/*   Updated: 2025/06/29 20:51:35 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;
	int		exit_status;

	pipex = ft_init(argc, argv, envp);
	ft_exec(pipex);
	exit_status = ft_wait_children(pipex);
	ft_free(pipex);
	return (exit_status);
}

static void	ft_execute(t_pipex *pipex)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < 2)
	{
		pid = fork();
		if (pid == -1)
			ft_error(pipex, "fork", strerror(errno), EXIT_FAILURE);
		if (pid == 0)
			ft_execute_child(pipex, i);
		pipex->pids[i] = pid;
		i++;
	}
}

static void	ft_execute_child(t_pipex *pipex, int i)
{
	ft_pipex_dup_infile(pipex, i);
	ft_pipex_dup_outfile(pipex, i);
	ft_pipex_close(pipex);
	

	execve(pipex->path_cmd, pipex->args_cmd, pipex->envp);
	ft_child_error();
}

static void	ft_pipex_close(t_pipex *pipex)
{
	close(pipex->pipes[0]);
	close(pipex->pipes[1]);
}

static void	ft_pipex_dup_infile(t_pipex *pipex, int i)
{
	if (i == 0)
	{
		if (dup2(pipex->infile, 0) == -1)
		{
			ft_free(pipex);
			ft_child_error(NULL, NULL, NULL, EXIT_FAILURE);
		}
	}
	else
	{
		if (dup2(pipex->pipes[0], 0) == -1)
		{
			ft_free(pipex);
			ft_child_error(NULL, NULL, NULL, EXIT_FAILURE);
		}
	}
}

static void	ft_pipex_dup_outfile(t_pipex *pipex, int i)
{
	if (i == 0)
	{
		if (dup2(pipex->pipes[1], 1) == -1)
		{
			ft_free(pipex);
			ft_child_error_exit(NULL, NULL, NULL, EXIT_FAILURE);
		}
	}
	else
	{
		if (dup2(pipex->outfile, 1) == -1)
		{
			ft_free(pipex);
			ft_child_error_exit(NULL, NULL, NULL, EXIT_FAILURE);
		}
	}
}

t_pipex	*ft_init(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;

	if (argc != 5)
		ft_error(NULL, "invalid input, you should do", "infile cmd1 cmd2 outfile", 2);
	pipex = ft_pipex_calloc(1, sizeof(t_pipex))
	pipex->argc = argc;
	pipex->argv = argv;
	pipex->envp = envp;
	pipex->infile = -1;
	pipex->outfile = -1;
	pipex->infile = ft_pipex_open(pipex, 1);
	pipex->outfile = ft_pipex_open(pipex, 0);
	pipex->pids = ft_pids_calloc(2, sizeof(pid_t), pipex);
	ft_pipex_pipes(pipex);
	return (pipex);
}

static void	ft_pipex_pipes(t_pipex *pipex)
{
	if (pipe(pipex->pipes) == -1)
		ft_error(pipex, "pipe", strerror(errno), EXIT_FAILURE);
}

static void	ft_pipex_open(t_pipex *pipex, int infile)
{
	int	fd;

	if (infile)
	{
		fd = open(pipex->argv[1], O_RDONLY);
		if (fd == -1)
			perror(pipex->argv[1]);
	}
	else
	{
		fd = open(pipex->argv[2], O_WRONLY | O_CREAT | O_TRUNC);
		if (fd == -1)
			perror(pipex->argv[2]);
	}
	return (fd);
}

static void	ft_pipex_calloc(size_t nmemb, size_t size)
{
	t_pipex	*pipex;

	pipex = ft_calloc(nmemb, size);
	if (!pipex)
		ft_error(NULL, "malloc", strerror(errno), EXIT_FAILURE);
	return (pipex);
}

static pid_t	*ft_pids_calloc(size_t nmemb, size_t size, t_pipex *pipex)
{
	pid_t	*pids;

	pids = ft_calloc(nmemb, size);
	if (!pids)
		ft_error(pipex, "malloc", strerror(errno), EXIT_FAILURE);
	return (pids);
}

