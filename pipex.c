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
  int     exit_status;

	if (argc < 5)
	{
		write(2, "You should do: ./pipex infile cmd1 cmd2 ... outfile\n", 52);
		return (1);
	}
  pipex = ft_init_pipex(argc, argv, envp);
  if (pipex->here_doc)
    ft_here_doc(pipex);
  ft_exec(pipex);
  exit_status = ft_wait_children(pipex);
  ft_free(pipex);
	return (exit_status);
}

void  ft_exec(t_pipex *pipex, int i)
{
  int i;
  pid_t pid;

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

void  ft_exec_child(t_pipex *pipex, int i)
{
  char **cmd_args;
  char *cmd_path;

  if (i == 0)
    dup2(pipex->infile, 0);
  else
    dup2(pipex->pipes[i - 1][0], 0);
  if (i == pipex->cmd_num - 1)
    dup2(pipex->outfile, 1);
  else
    dup2(pipex->pipes[i][1], 1);
  ft_close_pipes(pipex, 0);
  cmd_args = ft_split(pipex->argv[i + 2 + pipex->here_doc], ' ');
  if (!cmd_args || !cmd_args[0])
    ft_child_error_exit("malloc", "split or empty command", cmd_args, 127);
  cmd_path = ft_get_cmd_path(pipex, cmd_args[0]);
  if (!cmd_path)
    ft_child_error_exit(cmd_args[0], "command not found", cmd_args, 127);
  execve(cmd_path, cmd_args, pipex->envp);
  free(cmd_path);
  ft_child_error_exit(cmd_path, strerror(errno), cmd_args, 126);
}

char *ft_get_cmd_path(t_pipex *pipex, char *cmd)
{
  char  **paths;
  char  *envp_path;
  char  *full_path;
  int   i;

  if (ft_strchr(cmd, '/'))
  {
    if (access(cmd, X_OK) == 0)
      full_path = ft_strdup(cmd);
    else
      return (NULL);
    if (!full_path)
      return ()
  }
  envp_path = ft_find_path(pipex->envp);
  if (!envp_path)
    return (NULL);
  paths = ft_split(path_env, ':');
  if (!paths)
    return (NULL);
  i = 0;
  while (paths[i])
  {
    full_path = ft_strjoinjoin(paths[i], "/", cmd);
    if (!full_path)
    {
      ft_free_strs(paths);
      return (NULL);
    }
    if (access(full_path, X_OK) == 0)
    {
      ft_free_strs(paths);
      return (full_path);
    }
    free(full_path);
    i++;
  }
  ft_free_strs(paths);
  return (NULL);
}

void  ft_close_pipes(t_pipex *pipex, int limiter)
{
  int i;

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

void  ft_free_strs(char **strs)
{
  int i;

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

void  ft_free(t_pipex *pipex)
{
  int i;

  if (!pipex)
    return ;
  if (pipex->outfile != -1)
    close(outfile);
  if (pipex->infile != -1)
    close(infile);
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
  free(pipex->pids);
  free(pipex);
}

size_t  ft_strlen(char *str)
{
  int i;

  i = 0;
  while (str && str[i])
  {
    i++;
  }
  return (i);
}

void  ft_error_exit(t_pipex *pipex, char *function, char *msg, int exit_status)
{
  if (pipex)
    ft_free(pipex);
  if (function)
  {
    write(2, function, ft_strlen(function));
    write(2, ": ", 2);
  }
  if (msg)
    write(2, msg, ft_strlen(msg));
  write(2, "\n", 1);
  exit(exit_status);
}

void ft_child_error_exit(char *function, char *msg, char **to_free, int exit_status)
{
  if (function)
  {
    write(2, function, ft_strlen(function));
    write(2, ": ", 2);
  }
  if (msg)
    write(2, msg, ft_strlen(msg));
  write(2, "\n", 1);
  if (to_free)
    ft_free_strs(to_free);  
  exit(exit_status);
}

t_pipex *ft_pipex_calloc(size_t nmemb, size_t size)
{
  t_pipex *pipex;

  pipex = ft_calloc(nmemb, size);
  if (!pipex)
    ft_error_exit(NULL, "malloc", strerror(errno), EXIT_FAILURE);
  return (pipex);
}

int ft_open_pipex(t_pipex *pipex, int infile)
{
  int fd;
  int flags;

  if (infile)
  {
    if (pipex->here_doc)
      fd = open(".here_doc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else
      fd = open(pipex->argv[1], O_RDONLY);
  }
  else
  {
    if (pipex->here_doc)
      flags = O_WRONLY | O_CREAT | O_APPEND;
    else
      flags = O_WRONLY | O_CREAT | O_TRUNC;
    fd = open(pipex->argv[pipex->argc - 1], flags, 0644);
  }
  if (fd == -1)
    ft_error_exit(pipex, "open", strerror(errno), EXIT_FAILURE);
  return (fd);
}

void  ft_alloc_pipes(t_pipex *pipex)
{
  int i;

  pipex->pipes = malloc(sizeof(int *) * (pipex->cmd_num - 1));
  if (!pipex->pipes)
    ft_error_exit(pipex, "malloc", strerror(errno), EXIT_FAILURE);
  i = 0;
  while (i < pipex->cmd_num - 1)
  {
    pipex->pipes[i] = malloc(sizeof(int) * 2);
    if (!pipex->pipes[i])
      ft_error_exit(pipex, "malloc", strerror(errno), EXIT_FAILURE);
    if (pipe(pipex->pipes[i]) == -1))
    {
      ft_close_pipes(pipex, i);
      ft_error_exit(pipex, "pipe", strerror(errno), EXIT_FAILURE);
    }
    i++;
  }
}

t_pipex *ft_init_pipex(int argc, char **argv, char **envp)
{
  int i;
  t_pipex *pipex;

  pipex = ft_pipex_calloc(1, sizeof(t_pipex));
  pipex->argc = argc;
  pipex->argv = argv;
  pipex->envp = envp;
  pipex->here_doc = (ft_strncmp(argv[1], "here_doc", 8) == 0);
  pipex->limiter = argv[2];
  pipex->cmd_num = argc - 3 - pipex->here_doc;
  pipex->infile = -1;
  pipex->outfile = -1;
  pipex->infile = ft_open_pipex(pipex, 1);
  pipex->outfile = ft_open_pipex(pipex, 0);
  pipex->pids = ft_pids_calloc(pipex->cmd_num, sizeof(pid_t), pipex);
  ft_alloc_pipes(pipex);
  return (pipex);
}

pid_t *ft_pids_calloc(size_t nmemb, size_t size, t_pipex *pipex)
{
  pid_t *pids;

  pids = ft_calloc(nmemb * size);
  if (!pids)
    ft_error_exit(pipex, "malloc", strerror(errno), EXIT_FAILURE);
  return (pids);
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
