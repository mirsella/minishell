/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_everything.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:57:59 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/12 22:17:24 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_individual(char *line, int *i, t_list *env)
{
	char	*tmp;

	if (*line == '\'')
		tmp = expand_single_quote(line, i);
	else if (*line == '"')
		tmp = expand_double_quote(env, line, i);
	else if (*line == '$')
		tmp = expand_var(env, line, i);
	else
	{
		tmp = ft_strdup((char [2]){*line, 0});
		(*i)++;
	}
	if (!tmp)
		return (perror("malloc"), NULL);
	return (tmp);
}

char	*expand_everything(t_list *env, char *line)
{
	int		i;
	char	*str;
	char	*tmp;
	char	*expanded;

	i = 0;
	str = ft_strdup("");
	if (!str)
		return (perror("malloc"), NULL);
	tmp = expand_wildcards(line);
	if (!tmp)
		return (NULL);
	line = tmp;
	while (line[i])
	{
		expanded = expand_individual(line + i, &i, env);
		if (!expanded)
			return (free(str), free(line), NULL);
		tmp = ft_strjoin(str, expanded);
		free(expanded);
		free(str);
		str = tmp;
	}
	free(line);
	return (str);
}
