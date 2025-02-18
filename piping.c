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

void	here_doc_child(char *limiter, int fd)
{
	char	*line;
	size_t	limiter_len;

	limiter_len = ft_strlen(limiter);
	while (1)
	{
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, limiter_len) == 0
			&& line[limiter_len] == '\n')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	exit(EXIT_SUCCESS);
}

void exe(char **argv, char **directories, char **envp, int ncom_ishere[2])
{
    int pipes[ncom_ishere[0] - 1][2];
    pid_t pid, pid_final;
    int status;
    int i = 0;
	int j = 0;
	int fileout = 0;
    // Crear todos los pipes necesarios
    while (i < ncom_ishere[0] - 1) 
	{
        if (pipe(pipes[i]) == -1) 
		{
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        i++;
    }
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
		{ // Proceso hijo
            // Redirigir salida estándar al pipe si no es el último comando
            if (i < ncom_ishere[0] - 1) 
			{
                dup2(pipes[i][1], STDOUT_FILENO);
            }
			if (i == ncom_ishere[0] - 1) // caso en el que se redirige al archivo
			{
                if (ncom_ishere[1])
				{
					fileout = open(argv[ncom_ishere[0] + 2], O_WRONLY | O_CREAT | O_APPEND, 0644);
					if (fileout < 0)
					{
						perror("error file");
						exit(EXIT_FAILURE);
					}
					dup2(fileout, STDOUT_FILENO);
					close(fileout);
				}
				if (!ncom_ishere[1])
				{
					fileout = open(argv[ncom_ishere[0] + 2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (fileout < 0)
					{
						perror("error file");
						exit(EXIT_FAILURE);
					}
					dup2(fileout, STDOUT_FILENO);
					close(fileout);
				}
            }

            // Redirigir entrada estándar al pipe si no es el primer comando
            if (i > 0) 
			{
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
			if (i == 0) // caso en el que se lee del filein
			{
				if (ncom_ishere[1])
				{
					here_doc_child(argv[2], pipes[0][1]);
				}
				if (!ncom_ishere[1])
				{
					int filein = open(argv[1], O_RDONLY);
					if (filein < 0)
					{
						perror("error file");
						// Crear una pipe vacía
						int pipefd[2];
						if (pipe(pipefd) < 0) 
						{
							perror("Error creating pipe");
							exit(EXIT_FAILURE);
						}
						// Cerrar el extremo de escritura para que la pipe esté vacía
						close(pipefd[1]);

						// Usar el extremo de lectura de la pipe como entrada
						filein = pipefd[0];
					}
					dup2(filein, STDIN_FILENO);
					close(filein);
				}
			}

            // Cerrar todos los extremos de los pipes en el proceso hijo
            j = 0;
            while (j < ncom_ishere[0] - 1) 
			{
                close(pipes[j][0]);
                close(pipes[j][1]);
                j++;
            }

            // Ejecutar el comando
            execute(argv[i + 2], directories, envp);
        }

        // Guardar el PID del último comando para usarlo con waitpid
        if (i == ncom_ishere[0] - 1) 
		{
            pid_final = pid;
        }
        i++;
    }
	free_array(directories);
    // Cerrar todos los extremos de los pipes en el proceso padre
    i = 0;
    while (i < ncom_ishere[0] - 1) 
	{
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }

    pid_t wpid;       // para saber qué hijo terminó
int exit_code = 0; // para guardar el código de salida del último comando

// Esperar a los ncom_ishere[0] hijos, en el orden que terminen
i = 0;
while (i < ncom_ishere[0])
{
    wpid = waitpid(-1, &status, 0);
    if (wpid < 0)
    {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }

    // Si el PID que acaba de terminar es el del último comando, guardamos su status
    if (wpid == pid_final)
    {
        if (WIFEXITED(status)) //evalua si termina normal
            exit_code = WEXITSTATUS(status);
        else
            exit_code = 1; // caso en que no terminó con exit normal
    }
    i++;
}

// Al salir del bucle, nos vamos con exit_code, que es el del último comando
exit(exit_code);
}