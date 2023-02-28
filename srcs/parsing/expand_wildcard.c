/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:44:30 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/28 13:39:05 by lgillard         ###   ########.fr       */
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

int	is_wildcard(char *line)
{
	int		i;

	i = 0;
	while (line[i] && !isspace(line[i]) && line[i] != '*')
	{
		if (line[i] == '\'' || line[i] == '"')
			i += skip_quotes(line + i);
		else
			i++;
	}
	if (line[i] == '*')
		return (1);
	return (0);
}

char	*get_pattern(char *line, int *index)
{
	int				i;
	struct s_chars	chars;

	i = 0;
	chars.str = ft_strdup("");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (line[i] && !isspace(line[i]))
	{
		if (line[i] == '\'' || line[i] == '"')
			chars.tmp = get_in_quote(line + i, &i);
		else
			chars.tmp = ft_strdup((char []){line[i++], 0});
		if (!chars.tmp)
			return (free(chars.tmp), NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		chars.str = chars.joined;
	}
	*index += i;
	return (chars.str);
}

char	*expand_wildcard(char *line, int *index)
{
	char	*str;
	char	*pattern;
	int		i;

	i = 0;
	pattern = get_pattern(line, &i);
	if (!pattern)
		return (NULL);
	*index += i;
	str = get_matching_files(pattern);
	if (!str)
		return (free(pattern), NULL);
	free(pattern);
	if (!*str)
		return (free(str), ft_strndup(line, i));
	return (str);
}

char	*expand_wildcards(char *line)
{
	int		i;
	char	*str;
	char	*tmp;

	i = 0;
	str = ft_strdup("");
	while (line[i])
	{
		if (is_wildcard(line + i))
			tmp = expand_wildcard(line + i, &i);
		else if (line[i] == '\'' || line[i] == '"')
		{
			tmp = ft_substr(line, i, skip_quotes(line + i));
			i += skip_quotes(line + i);
		}
		else
			tmp = ft_strdup((char []){line[i++], 0});
		if (!tmp)
			return (free(str), NULL);
		str = ft_strjoin_free(str, tmp);
		if (!str)
			return (NULL);
	}
	return (str);
}
