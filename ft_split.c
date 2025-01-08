/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 11:33:07 by luialvar          #+#    #+#             */
/*   Updated: 2024/10/14 11:51:39 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	size;

	if (!s)
		return (NULL);
	size = ft_strlen (s);
	if (start >= size)
	{
		sub = malloc (1);
		if (!sub)
			return (NULL);
		sub[0] = '\0';
		return (sub);
	}
	if (len + start > size)
	{
		len = size - start;
	}
	sub = (char *)malloc (len + 1);
	if (!sub)
		return (NULL);
	ft_memcpy(sub, s + start, len);
	sub[len] = '\0';
	return (sub);
}

static int	ft_count_strings(const char*s, char c)
{
	size_t		i;
	int			a;
	size_t		len;

	i = 0;
	a = 0;
	len = ft_strlen(s);
	while (i < len)
	{
		if (s[i] != c)
		{
			a++;
			while (s[i] != c && i < ft_strlen (s))
				i++;
		}
		while (s[i] == c)
			i++;
	}
	return (a);
}

static void	ft_free(char **retorno, int i)
{
	while (i >= 0)
	{
		free(retorno[i]);
		retorno[i] = NULL;
		i--;
	}
	free(retorno);
}

static int	meter(const char*s, char c, char**retorno)
{
	size_t		i;
	int			a;
	int			start;

	i = 0;
	a = 0;
	while (i < ft_strlen (s))
	{
		if (s[i] != c)
		{
			start = i;
			a++;
			while (s[i] != c && i < ft_strlen (s))
				i++;
			retorno[a - 1] = ft_substr (s, start, i - start);
			if (!(retorno[a - 1]))
			{
				ft_free(retorno, a - 1);
				return (0);
			}
		}
		while (s[i] == c)
			i++;
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	int		num_strings;
	char	**retorno;
	int		bueno;

	if (!s)
		return (NULL);
	num_strings = ft_count_strings (s, c);
	retorno = malloc(sizeof(char *) * (num_strings + 1));
	if (!retorno)
		return (NULL);
	bueno = meter (s, c, retorno);
	if (!bueno)
	{
		free(retorno);
		return (NULL);
	}
	retorno[num_strings] = NULL;
	return (retorno);
}

/*
int main(void)
{
    char *str = "Este es un test para ft_split";
    char **result;
    int i = 0;

    result = ft_split(str, ' ');

    if (!result)
        return (1);

    while (result[i])
    {
        printf("%s\n", result[i]);
        free(result[i]);
        i++;
    }
    free(result);
    return (0);
}*/
