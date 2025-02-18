/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 10:47:03 by luialvar          #+#    #+#             */
/*   Updated: 2024/12/12 17:08:43 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	try(char *full_path, char **commands, char **directories, char **envp)
{
	if (access(full_path, X_OK) == 0)
	{
		execve(full_path, commands, envp);
		perror("execve error");
		free(full_path);
		free_array(commands);
		free_array(directories);
		exit(EXIT_FAILURE);
	}
	free(full_path);
}

char	**handle_params_allocation(char *command, char **directories)
{
	char	**commands;

	commands = ft_split(command, ' ');
	if (!commands)
	{
		perror("malloc error");
		free_array(directories);
		exit(EXIT_FAILURE);
	}
	return (commands);
}

void	execute(char *command, char **directories, char **envp)
{
	char	**params;
	char	*full_path;
	int		i;

	params = handle_params_allocation(command, directories);
	i = 0;
	while (directories[i] != NULL)
	{
		full_path = build_full_path(directories[i], params[0]);
		if (!full_path)
		{
			free_array(params);
			free_array(directories);
			perror("malloc error");
			exit(EXIT_FAILURE);
		}
		try(full_path, params, directories, envp);
		i++;
	}
	free_array(params);
	free_array(directories);
	write(STDERR_FILENO, command, ft_strlen(command));
	write(STDERR_FILENO, ": command not found\n", 20);
	exit(127);
}

int	check_argc_heredoc(int argc, char **argv) //checkeos iniciales
{
	int	fileout;

	fileout = 0;
	if (argc < 5)
	{
		perror("Invalid number of parameters");
		exit(EXIT_FAILURE);
	}
	if (ft_strncmp("here_doc", argv[1], 8) == 0)
		fileout = 1;
	return (fileout);
}

int	main(int argc, char **argv, char **envp)
{
	int		is_here_doc;
	char	**directories;
	int		ncom_isheredoc[2];

	is_here_doc = check_argc_heredoc(argc, argv);
	directories = find_directories(envp);
	if (!directories)
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	ncom_isheredoc[0] = argc - 3;
	ncom_isheredoc[1] = is_here_doc;
	exe(argv, directories, envp, ncom_ishere);
}