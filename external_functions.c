/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 19:54:45 by luialvar          #+#    #+#             */
/*   Updated: 2024/11/14 20:04:35 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_array(char **directories)
{
	int	i;

	i = 0;
	while (directories[i] != NULL)
	{
		free(directories[i]);
		i++;
	}
	free(directories);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len;
	size_t	i;

	len = 0;
	i = 0;
	while (dst[len] && len < size)
		len++;
	while (src[i] && (len + i + 1) < size)
	{
		dst[len + i] = src[i];
		i++;
	}
	if (len < size)
		dst[len + i] = '\0';
	return (len + ft_strlen(src));
}

/* 
Da las posibles rutas donde se pueden encontrar los archivos ejecutables
dentro de PATH
*/
char	**find_directories(char **envp)
{
	char	string_path[6];
	int		pos;
	char	**directories;
	char	*path_env;

	pos = 0;
	ft_strlcpy(string_path, "PATH=", sizeof(string_path));
	while (envp[pos] != NULL)
	{
		if (ft_strncmp(string_path, envp[pos], 5) == 0)
		{
			path_env = envp[pos] + 5;
			directories = ft_split(path_env, ':');
			if (!directories)
			{
				perror("malloc error");
				exit(EXIT_FAILURE);
			}
			return (directories);
		}
		pos++;
	}
	perror("No path defined");
	exit(EXIT_FAILURE);
}

char	*build_full_path(const char *directory, const char *command)
{
	size_t	total_size;
	char	*full_path;

	total_size = ft_strlen(directory) + ft_strlen(command) + 2;
	full_path = malloc(total_size);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, directory, total_size);
	ft_strlcat(full_path, "/", total_size);
	ft_strlcat(full_path, command, total_size);
	return (full_path);
}
