/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 00:43:50 by feazeved          #+#    #+#             */
/*   Updated: 2025/06/30 22:20:02 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_pipex	*ft_init_pipex(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;

	if (argc < 4)
		ft_error_exit(NULL, "invalid input, you should do",
			"infile cmd1 cmd2 ... outfile", 2);
	pipex = ft_pipex_calloc(1, sizeof(t_pipex));
	pipex->argc = argc;
	pipex->argv = argv;
	pipex->envp = envp;
	pipex->cmd_num = argc - 3;
	pipex->infile = -1;
	pipex->outfile = -1;
	pipex->infile = ft_open_pipex(pipex, 1);
	pipex->outfile = ft_open_pipex(pipex, 0);
	pipex->pids = ft_pids_calloc(pipex->cmd_num, sizeof(pid_t), pipex);
	ft_alloc_pipes(pipex);
	return (pipex);
}

void	ft_alloc_pipes(t_pipex *pipex)
{
	int	i;

	pipex->pipes = malloc(sizeof(int *) * (pipex->cmd_num - 1));
	if (!pipex->pipes)
		ft_error_exit(pipex, "malloc", strerror(errno), EXIT_FAILURE);
	i = 0;
	while (i < pipex->cmd_num - 1)
	{
		pipex->pipes[i] = malloc(sizeof(int) * 2);
		if (!pipex->pipes[i])
			ft_error_exit(pipex, "malloc", strerror(errno), EXIT_FAILURE);
		if (pipe(pipex->pipes[i]) == -1)
		{
			ft_close_pipes(pipex, i);
			ft_error_exit(pipex, "pipe", strerror(errno), EXIT_FAILURE);
		}
		i++;
	}
}

t_pipex	*ft_pipex_calloc(size_t nmemb, size_t size)
{
	t_pipex	*pipex;

	pipex = ft_calloc(nmemb, size);
	if (!pipex)
		ft_error_exit(NULL, "malloc", strerror(errno), EXIT_FAILURE);
	return (pipex);
}

int	ft_open_pipex(t_pipex *pipex, int infile)
{
	int	fd;
	int	flags;

	if (infile)
	{
		fd = open(pipex->argv[1], O_RDONLY);
		if (fd == -1)
			perror(pipex->argv[1]);
	}
	else
	{
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(pipex->argv[pipex->argc - 1], flags, 0644);
		if (fd == -1)
			perror(pipex->argv[pipex->argc - 1]);
	}
	return (fd);
}

pid_t	*ft_pids_calloc(size_t nmemb, size_t size, t_pipex *pipex)
{
	pid_t	*pids;

	pids = ft_calloc(nmemb, size);
	if (!pids)
		ft_error_exit(pipex, "malloc", strerror(errno), EXIT_FAILURE);
	return (pids);
}
