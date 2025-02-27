/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:26:45 by luialvar          #+#    #+#             */
/*   Updated: 2025/01/07 15:36:02 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	handle_tofile(int *ncom_ishere, char **argv)
{
	int	fileout;

	fileout = -1;
	if (ncom_ishere[1])
		fileout = open(argv[ncom_ishere[0] + 2], O_WRONLY | O_CREAT
				| O_APPEND, 0644);
	else
		fileout = open(argv[ncom_ishere[0] + 2], O_WRONLY | O_CREAT
				| O_TRUNC, 0644);
	if (fileout < 0)
	{
		perror("error file");
		exit(EXIT_FAILURE);
	}
	dup2(fileout, STDOUT_FILENO);
	close(fileout);
}

void	handle_fromfile(int pipes[][2], int *ncom_ishere, char **argv)
{
	int		pipefd[2];
	int		filein;

	filein = -1;
	if (ncom_ishere[1])
		here_doc_child(argv[2], pipes[0][1]);
	else
	{
		filein = open(argv[1], O_RDONLY);
		if (filein < 0)
		{
			perror("error file");
			if (pipe(pipefd) < 0)
			{
				perror("Error creating pipe");
				exit(EXIT_FAILURE);
			}
			close(pipefd[1]);
			filein = pipefd[0];
		}
		dup2(filein, STDIN_FILENO);
		close(filein);
	}
}

void	redirect(int pipes[][2], int *ncom_ishere, int current, char **argv)
{
	int	i;

	i = 0;
	if (current < ncom_ishere[0] - 1)
		dup2(pipes[current][1], STDOUT_FILENO);
	if (current > 0)
		dup2(pipes[current - 1][0], STDIN_FILENO);
	if (current == ncom_ishere[0] - 1)
		handle_tofile(ncom_ishere, argv);
	if (current == 0)
		handle_fromfile(pipes, ncom_ishere, argv);
	while (i < ncom_ishere[0] - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

pid_t	forking(int pipes[][2], int *ncom_ishere, char **argv, char ***mix)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < ncom_ishere[0])
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			redirect(pipes, ncom_ishere, i, argv);
			execute(argv[i + 2], mix[0], mix[1]);
		}
		i++;
	}
	return (pid);
}

void	run(char **argv, char **directories, char **envp, int ncom_ishere[2])
{
	int		(*pipes)[2];
	pid_t	pid;
	int		i;
	char	**mix[2];

	pipes = malloc(sizeof(int [2]) * (ncom_ishere[0] - 1));
	if (!pipes)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	mix[0] = directories;
	mix[1] = envp;
	create_pipes(pipes, ncom_ishere[0]);
	pid = forking(pipes, ncom_ishere, argv, mix);
	free_array(directories);
	while (i < ncom_ishere[0] - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	free(pipes);
	returning(ncom_ishere, pid);
}
