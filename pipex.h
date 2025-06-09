/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 21:07:20 by feazeved          #+#    #+#             */
/*   Updated: 2025/06/09 23:48:42 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

typedef struct s_pipex
{
	char	**argv;
	char	**envp;
	int		fd_in;
	int		fd_out;
	int		pipes_fd[2];
	char	**paths;
	char	**args_cmd1;
	char	**args_cmd2;
	char	*path_cmd1;
	char	*path_cmd2;
}	t_pipex;

void	ft_error(t_pipex *pipex, int check);
t_pipex	*ft_init_pipex(char **argv);
char	*ft_find_path(char **envp);
char	**ft_get_paths(t_pipex *pipex);
int		ft_strncmp(char *s1, char *s2, int n);
char	**ft_split(const char *s, char c);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_exec_cmd(t_pipex *pipex);
char	*ft_find_cmd(char *cmd, t_pipex *pipex);
void	ft_parsing(t_pipex *pipex);
char	*ft_strjoin(char *s1, char *s2, t_pipex *pipex);
int	ft_strlen(char *s1);

#endif
