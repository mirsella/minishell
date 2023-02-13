/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:44:30 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/13 21:51:01 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>

struct s_chars
{
	char	*tmp;
	char	*str;
	char	*joined;
};

int	ismeta(char c)
{
	if (c == '|' || c == '&')
		return (1);
	return (0);
}

int	is_wildcard(char *line)
{
	int	i;

	i = 0;
	while (line[i] && !isspace(line[i]) && line[i] != '*')
	{
		if (line[i] == '\'' || line[i] == '\"')
			i += skip_quotes(line + i);
		else
			i++;
	}
	if (line[i] == '*')
		return (1);
	return (0);
}

char	*get_pattern(char *line, int *index, t_list *env)
{
	int				i;
	struct s_chars	chars;

	i = 0;
	chars.str = NULL;
	while (line[i] && !isspace(line[i]) && !ismeta(line[i]))
	{
		chars.tmp = expand_individual(line + i, &i, env);
		if (!chars.tmp)
			return (NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		if (!chars.joined)
			return (NULL);
		chars.str = chars.joined;
	}
	*index += i;
	return (chars.str);
}

char	*get_wildcard(char *line, int *i, t_list *env)
{
	char	*str;
	char	*pattern;
	int		j;

	j = 0;
	pattern = get_pattern(line, &j, env);
	if (!pattern)
		return (NULL);
	*i += j;
	str = get_matching_files(pattern);
	if (!str)
		return (free(pattern), NULL);
	free(pattern);
	if (!*str)
		return (free(str), ft_strndup(line, j));
	return (str);
}

char	*expand_wildcards(char *line, t_list *env)
{
	struct s_chars	chars;
	int				i;

	i = 0;
	chars.str = ft_strdup("");
	while (line[i])
	{
		if (is_wildcard(line + i))
			chars.tmp = get_wildcard(line + i, &i, env);
		else if (line[i] == '\'' || line[i] == '\"')
		{
			chars.tmp = ft_strndup(line + i, skip_quotes(line + i));
			i += skip_quotes(line + i);
		}
		else
			chars.tmp = ft_strdup((char []){line[i++], '\0'});
		if (!chars.tmp)
			return (perror("malloc"), NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		if (!chars.joined)
			return (perror("malloc"), NULL);
		chars.str = chars.joined;
	}
	return (chars.str);
}
