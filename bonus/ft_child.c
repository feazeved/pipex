/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_child.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 00:39:19 by feazeved          #+#    #+#             */
/*   Updated: 2025/06/29 20:51:37 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_exec_child(t_pipex *pipex, int i)
{
	char	**cmd_args;
	char	*cmd_path;

	ft_pipex_dup_in(pipex, i);
	ft_pipex_dup_out(pipex, i);
	ft_close_pipes(pipex, 0);
	ft_prep_cmd(pipex, i, &cmd_args, &cmd_path);
	execve(cmd_path, cmd_args, pipex->envp);
	free(cmd_path);
	ft_free(pipex);
	ft_child_error_exit(cmd_args[0], strerror(errno), cmd_args, 126);
}

void	ft_prep_cmd(t_pipex *pipex, int i, char ***cmd_args, char **cmd_path)
{
	char	*cmd_str;

	cmd_str = pipex->argv[i + 2 + pipex->here_doc];
	if (!ft_countwords(cmd_str, ' '))
	{
		ft_free(pipex);
		ft_child_error_exit(NULL, "invalid command", NULL, 127);
	}
	*cmd_args = ft_split(cmd_str, ' ');
	if (!*cmd_args)
	{
		ft_free(pipex);
		ft_child_error_exit("malloc", strerror(errno), NULL, EXIT_FAILURE);
	}
	*cmd_path = ft_get_cmd_path(pipex, *cmd_args);
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

char	*ft_get_cmd_path(t_pipex *pipex, char **cmd)
{
	char	**paths;
	char	*envp_path;
	char	*full_path;

	if (ft_strchr(cmd[0], '/'))
		return (ft_absolute_path(pipex, cmd));
	envp_path = ft_find_path(pipex->envp);
	if (!envp_path)
	{
		ft_free(pipex);
		ft_child_error_exit(cmd[0], "command not found", cmd, 127);
	}
	paths = ft_split(envp_path, ':');
	if (!paths)
	{
		ft_free(pipex);
		ft_child_error_exit("malloc", strerror(errno), cmd, EXIT_FAILURE);
	}
	full_path = ft_get_full_path(pipex, paths, cmd);
	return (ft_free_strs(paths), full_path);
}

char	*ft_get_full_path(t_pipex *pipex, char **paths, char **cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoinjoin(paths[i], "/", cmd[0]);
		if (!full_path)
		{
			ft_free_strs(paths);
			ft_free(pipex);
			ft_child_error_exit("malloc", strerror(errno), cmd, EXIT_FAILURE);
		}
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	ft_free_strs(paths);
	ft_free(pipex);
	ft_child_error_exit(cmd[0], "command not found", cmd, 127);
	return (NULL);
}
