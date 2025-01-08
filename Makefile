# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/09 18:34:39 by luialvar          #+#    #+#              #
#    Updated: 2024/11/14 19:56:08 by luialvar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nombre del ejecutable
NAME = pipex

# Lista de archivos fuente (incluyendo los de sorting_operations)
SRCS = pipex.c \
       gnl/get_next_line.c \
       gnl/get_next_line_utils.c \
       external_functions.c \
       piping.c \
       ft_split.c \

# Objetos (los archivos .o generados a partir de los .c)
OBJ = $(SRCS:.c=.o)

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Reglas principales

# Regla 'all': se encarga de generar el ejecutable
all: $(NAME)

# Regla para generar el ejecutable a partir de los objetos
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

# Regla implícita para generar los objetos a partir de los archivos fuente
$(OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza de archivos objeto
clean:
	rm -f $(OBJ)

# Limpieza completa, incluyendo el ejecutable
fclean: clean
	rm -rf $(NAME)

# Regenerar todo
re: fclean all
