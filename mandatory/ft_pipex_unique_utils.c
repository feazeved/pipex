/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex_unique_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 00:48:58 by feazeved          #+#    #+#             */
/*   Updated: 2025/06/30 23:46:04 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_strjoinjoin(char *str1, char *str2, char *str3)
{
	int		j;
	int		i;
	char	*new;

	new = malloc(ft_strlen(str1) + ft_strlen(str2) + ft_strlen(str3) + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (str1 && str1[i])
	{
		new[i] = str1[i];
		i++;
	}
	j = 0;
	while (str2 && str2[j])
		new[i++] = str2[j++];
	j = 0;
	while (str3 && str3[j])
		new[i++] = str3[j++];
	new[i] = '\0';
	return (new);
}

char	*ft_absolute_path(t_pipex *pipex, char **cmd)
{
	char	*full_path;

	if (access(cmd[0], X_OK) == 0)
	{
		full_path = ft_strdup(cmd[0]);
		if (!full_path)
		{
			ft_free(pipex);
			ft_child_error_exit("malloc", strerror(errno), cmd, EXIT_FAILURE);
		}
		return (full_path);
	}
	else
	{
		ft_free(pipex);
		ft_child_error_exit(cmd[0], strerror(errno), cmd, 126);
		return (NULL);
	}
}

int	ft_countwords(char *str, char c)
{
	int	counter;
	int	i;

	i = 0;
	counter = 0;
	while (str && str[i])
	{
		while (str[i] == c)
			i++;
		if (str[i])
			counter++;
		while (str[i] != c && str[i])
			i++;
	}
	return (counter);
}

void	ft_pipex_dup_out(t_pipex *pipex, int i)
{
	if (i == pipex->cmd_num - 1)
	{
		if (dup2(pipex->outfile, 1) == -1)
		{
			ft_free(pipex);
			ft_child_error_exit(NULL, NULL, NULL, EXIT_FAILURE);
		}
	}
	else
	{
		if (dup2(pipex->pipes[i][1], 1) == -1)
		{
			ft_free(pipex);
			ft_child_error_exit(NULL, NULL, NULL, EXIT_FAILURE);
		}
	}
}

void	ft_pipex_dup_in(t_pipex *pipex, int i)
{
	if (i == 0)
	{
		if (dup2(pipex->infile, 0) == -1)
		{
			ft_free(pipex);
			ft_child_error_exit(NULL, NULL, NULL, EXIT_FAILURE);
		}
	}
	else
	{
		if (dup2(pipex->pipes[i - 1][0], 0) == -1)
		{
			ft_free(pipex);
			ft_child_error_exit(NULL, NULL, NULL, EXIT_FAILURE);
		}
	}
}
