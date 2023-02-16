/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expantion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:57:59 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/16 21:42:57 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <ctype.h>

struct s_chars
{
	char	*str;
	char	*tmp;
	char	*joined;
};

char	*expand_one(char *line, t_list *env, int *index)
{
	char	*tmp;

	if (is_wildcard(line, env))
		tmp = expand_wildcard(line, index, env);
	else if (*line == '\'')
		tmp = expand_single_quote(line, index);
	else if (*line == '"')
		tmp = expand_double_quote(line, index, env);
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
