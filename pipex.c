/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 21:09:36 by feazeved          #+#    #+#             */
/*   Updated: 2025/06/10 00:45:04 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

int	main(int argc, char *argv[], char **envp)
{
	t_pipex	*pipex;

	if (argc != 5)
	{
		write(2, "You should do: ./pipex infile cmd1 cmd2 outfile\n", 48);
		return (1);
	}
	pipex = ft_init_pipex(argv);
	pipex->argv = argv;
	pipex->envp = envp;
	pipex->paths = ft_get_paths(pipex);
	ft_parsing(pipex);
	ft_exec_cmd(pipex);
	ft_error(pipex, 0);
	return (0);
}

void	ft_exec_cmd(t_pipex *pipex)
{
	pid_t	id1;
	pid_t	id2;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		ft_error(pipex, 1);
	}
	id1 = fork();
	if (id1 == -1)
	{
		perror("fork");
		ft_error(pipex, 1);
	}
	if (id1 == 0)
	{
		dup2(pipex->fd_in, 0);
		dup2(pipe_fd[1], 1);
		close(pipe_fd[0]);
		execve(pipex->path_cmd1, pipex->args_cmd1, pipex->envp);
		perror("execve");
		ft_error(pipex, 1);
	}
	id2 = fork();
	if (id2 == -1)
	{
		perror("fork");
		ft_error(pipex, 1);
	}
	if (id2 == 0)
	{
		dup2(pipe_fd[0], 0);
		dup2(pipex->fd_out, 1);
		close(pipe_fd[1]);
		execve(pipex->path_cmd2, pipex->args_cmd2, pipex->envp);
		perror("execve");
		ft_error(pipex, 1);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(id1, NULL, 0);
	waitpid(id2, NULL, 0);
}

char	*ft_find_cmd(char *cmd, t_pipex *pipex)
{
	char	*temp;
	char	*all;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	i = 0;
	while (pipex->paths[i])
	{
		temp = ft_strjoin(pipex->paths[i], "/", pipex);
		all = ft_strjoin(temp, cmd, pipex);
		free(temp);
		if (access(all, X_OK) == 0)
			return (all);
		free(all);
		i++;
	}
	return (NULL);
}

void	ft_parsing(t_pipex *pipex)
{
	pipex->args_cmd1 = ft_split(pipex->argv[2], ' ');
	pipex->args_cmd2 = ft_split(pipex->argv[3], ' ');
	if (!pipex->args_cmd1 || !pipex->args_cmd2)
	{
		perror("malloc");
		ft_error(pipex, 1);
	}
	pipex->path_cmd1 = ft_find_cmd(pipex->args_cmd1[0], pipex);
	pipex->path_cmd2 = ft_find_cmd(pipex->args_cmd2[0], pipex);
	if (!pipex->path_cmd1 || !pipex->path_cmd2)
	{
		write(2, "Command not found\n", 18);
		ft_error(pipex, 1);
	}
}

char	**ft_get_paths(t_pipex *pipex)
{
	char	*path;
	char	**splitted;

	path = ft_find_path(pipex->envp);
	if (!path)
	{
		write(2, "No path found\n", 14);
		ft_error(pipex, 1);
	}
	splitted = ft_split(path, ':');
	if (!splitted)
	{
		perror("malloc");
		ft_error(pipex, 1);
	}
	return (splitted);
}

char	*ft_find_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

t_pipex	*ft_init_pipex(char **argv)
{
	t_pipex *pipex;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
  pipex->fd_in = -1;
  pipex->fd_out = -1;
  pipex->paths = NULL;
  pipex->args_cmd1 = NULL;
  pipex->args_cmd2 = NULL;
  pipex->path_cmd1 = NULL;
  pipex->path_cmd2 = NULL;
	pipex->fd_in = open(argv[1], O_RDONLY);
	if (pipex->fd_in == -1)
	{
		perror("open");
		ft_error(pipex, 1);
	}
	pipex->fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->fd_out == -1)
	{
		perror("open");
		ft_error(pipex, 1);
	}
	return (pipex);
}

void	ft_error(t_pipex *pipex, int check)
{
	int	i;

	if (pipex->fd_in != -1)
		close(pipex->fd_in);
	if (pipex->fd_out != -1)
		close(pipex->fd_out);
	i = 0;
	while (pipex->paths && pipex->paths[i])
		free(pipex->paths[i++]);
	free(pipex->paths);
	i = 0;
	while (pipex->args_cmd1 && pipex->args_cmd1[i])
		free(pipex->args_cmd1[i++]);
	free(pipex->args_cmd1);
	i = 0;
	while (pipex->args_cmd2 && pipex->args_cmd2[i])
		free(pipex->args_cmd2[i++]);
	free(pipex->args_cmd2);
	free(pipex);
	if (check)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
