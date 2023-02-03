/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 23:13:50 by mirsella          #+#    #+#             */
/*   Updated: 2022/11/15 14:04:05 by lgillard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	count_words(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static char	*getword(char const *s, int i, char c)
{
	char	*res;
	int		j;
	int		wordlen;

	j = 0;
	wordlen = 0;
	while (s[i + wordlen] && s[i + wordlen] != c)
		wordlen++;
	res = malloc(sizeof(char) * wordlen + 1);
	if (!res)
		return (NULL);
	while (s[i] && s[i] != c)
	{
		res[j] = s[i];
		i++;
		j++;
	}
	res[j] = 0;
	return (res);
}

static void	freetab(char **res, int i)
{
	while (i >= 0)
	{
		free(res[i]);
		i--;
	}
	free(res);
}

static char	**loop(char const *s, char c, char **tab)
{
	int		i;
	int		wordcount;

	i = 0;
	wordcount = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
		{
			tab[wordcount] = getword(s, i, c);
			if (!tab[wordcount])
			{
				freetab(tab, wordcount);
				return (NULL);
			}
			wordcount++;
		}
		while (s[i] && s[i] != c)
			i++;
	}
	tab[wordcount] = 0;
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;

	if (!s)
		return (NULL);
	tab = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!tab)
		return (NULL);
	return (loop(s, c, tab));
}
