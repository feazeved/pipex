/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 21:07:20 by feazeved          #+#    #+#             */
/*   Updated: 2025/06/29 19:44:00 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define BUFFER_SIZE 1024
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>

typedef struct s_pipex
{
	int		argc;
	char	**argv;
	char	**envp;
	int		infile;
	int		outfile;
	int		cmd_num;
	int		**pipes;
	pid_t	*pids;
}	t_pipex;

int		ft_wait_children(t_pipex *pipex);
void	ft_exec(t_pipex *pipex);
void	ft_exec_child(t_pipex *pipex, int i);
void	ft_pipex_dup_in(t_pipex *pipex, int i);
void	ft_pipex_dup_out(t_pipex *pipex, int i);
int		ft_countwords(char *str, char c);
char	*ft_get_cmd_path(t_pipex *pipex, char **cmd);
char	*ft_get_full_path(t_pipex *pipex, char **paths, char **cmd);
char	*ft_strjoinjoin(char *str1, char *str2, char *str3);
void	ft_close_pipes(t_pipex *pipex, int limiter);
void	ft_free_strs(char **strs);
void	ft_free(t_pipex *pipex);
void	ft_error_exit(t_pipex *pipex, char *func, char *msg, int status);
void	ft_child_error_exit(char *func, char *msg, char **to_free, int status);
t_pipex	*ft_pipex_calloc(size_t nmemb, size_t size);
int		ft_open_pipex(t_pipex *pipex, int infile);
void	ft_alloc_pipes(t_pipex *pipex);
t_pipex	*ft_init_pipex(int argc, char **argv, char **envp);
pid_t	*ft_pids_calloc(size_t nmemb, size_t size, t_pipex *pipex);
char	*ft_find_path(char **envp);
char	**ft_split(const char *s, char c);
char	*ft_str_join_and_free(char *old, char *buffer, t_pipex *pipex);
int		ft_clean_buffer(char *buffer);
int		ft_strlen_line(char *str);
char	*get_next_line(int fd, t_pipex *pipex);
void	ft_initializer(char **line, int *br, int *num_read);
char	*ft_strchr(char *str, char to_look);
char	*ft_strdup(char *str);
int		ft_strlen(char *s1);
void	*ft_calloc(size_t nmemb, size_t size);
int		ft_strncmp(char *s1, char *s2, int n);
char	*ft_absolute_path(t_pipex *pipex, char **cmd);
void	ft_prep_cmd(t_pipex *pipex, int i, char ***cmd_args, char **cmd_path);

#endif
