/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expantion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:57:59 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/17 00:17:31 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <ctype.h>
#include <stdio.h>

struct s_chars
{
	char	*str;
	char	*tmp;
	char	*joined;
};

char	*expand_one(char *line, t_list *env, int *index)
{
	char	*tmp;

	if (is_wildcard(line))
		tmp = expand_wildcard(line, index);
	else if (*line == '\'' || *line == '"')
		tmp = get_in_quote(line, index);
	else if (*line == '$')
		tmp = expand_var(env, line, index);
	else
	{
		tmp = ft_strdup((char []){*line, 0});
		(*index)++;
	}
	if (!tmp)
		return (NULL);
	return (tmp);
}

char	*expand_vars_in_double_quote(char *line, int *index, t_list *env)
{
	int				i;
	struct s_chars	chars;

	i = 1;
	chars.str = ft_strdup("\"");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (line[i] && line[i] != '"')
	{
		if (line[i] == '$')
			chars.joined = expand_var(env, line + i, &i);
		else
			chars.joined = ft_strdup((char [2]){line[i++], 0});
		if (!chars.joined)
			return (perror("malloc"), NULL);
		chars.tmp = ft_strjoin_free(chars.str, chars.joined);
		if (!chars.tmp)
			return (perror("malloc"), NULL);
		chars.str = chars.tmp;
	}
	*index += i + 1;
	chars.tmp = ft_strjoin_free(chars.str, ft_strdup("\""));
	if (!chars.tmp)
		return (perror("malloc"), NULL);
	return (chars.tmp);
}

char	*expand_vars(char *line, t_list *env)
{
	struct s_chars	chars;
	int				i;

	i = 0;
	chars.str = ft_strdup("");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (line[i])
	{
		if (line[i] == '$')
			chars.tmp = expand_var(env, line + i, &i);
		else if (line[i] == '"')
			chars.tmp = expand_vars_in_double_quote(line + i, &i, env);
		else if (line[i] == '\'')
		{
			chars.tmp = ft_substr(line, i, skip_quotes(line + i));
			i += skip_quotes(line + i);
		}
		else
			chars.tmp = ft_strdup((char []){line[i++], 0});
		if (!chars.tmp)
			return (free(chars.str), perror("malloc"), NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		chars.str = chars.joined;
	}
	return (chars.str);
}

char	*remove_quotes(char *line)
{
	struct s_chars	chars;
	int				i;

	i = 0;
	chars.str = ft_strdup("");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
			chars.tmp = get_in_quote(line + i, &i);
		else
			chars.tmp = ft_strdup((char []){line[i++], 0});
		if (!chars.tmp)
			return (free(chars.str), NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		chars.str = chars.joined;
	}
	return (chars.str);
}

char	*expand_everything(char *line, t_list *env)
{
	int				i;
	struct s_chars	chars;

	i = 0;
	chars.str = ft_strdup("");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (line[i])
	{
		chars.tmp = expand_one(line + i, env, &i);
		if (!chars.tmp)
			return (free(chars.str), NULL);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		chars.str = chars.joined;
	}
	return (chars.str);
}
