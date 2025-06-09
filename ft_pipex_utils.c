/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 21:24:47 by feazeved          #+#    #+#             */
/*   Updated: 2025/06/09 23:45:17 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_strncmp(char *s1, char *s2, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		if (!s1[i])
			return (0);
		i++;
	}
	return (0);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			i;
	size_t			a;
	unsigned char	*ptr;

	i = 0;
	a = nmemb * size;
	if (size && (a / size != nmemb))
		return (NULL);
	ptr = malloc(a);
	if (!ptr)
		return (NULL);
	while (i < a)
	{
		ptr[i] = 0;
		i++;
	}
	return (ptr);
}

int	ft_strlen(char *s1)
{
	int	i;

	i = 0;
	if (!s1)
		return (0);
	while (s1[i])
		i++;
	return (i);
}
	
char	*ft_strjoin(char *s1, char *s2, t_pipex *pipex)
{
	char	*join;
	int		i;
	int		j;

	join = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!join)
	{
		perror("Error when allocating memory");
		ft_error(pipex, 1);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (s1[i])
	{
		join[i] = s1[i];
		i++;
	}
	while (s2[j])
		join[i++] = s2[j++];
	join[i] = '\0';
	return (join);
}
