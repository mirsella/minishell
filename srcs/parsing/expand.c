/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 23:42:59 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/12 22:22:55 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

struct s_chars
{
	char	*tmp;
	char	*str;
	char	*expanded;
};

static int	ismeta(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '&'
		|| c == '(' || c == '\'' || c == '"' || c == '$');
}

char	*expand_var(t_list *env, char *line, int *index)
{
	int		stop;
	char	*var;
	char	*tmp;

	stop = 1;
	while (line[stop] && !ismeta(line[stop]) && !isspace(line[stop]))
		stop++;
	if (stop == 1)
		return ((*index)++, ft_strdup("$"));
	*index += stop;
	tmp = ft_substr(line, 1, stop - 1);
	var = get_env_value(env, tmp);
	free(tmp);
	if (var == NULL)
		return (ft_strdup(""));
	return (ft_strdup(var));
}

char	*expand_single_quote(char *line, int *index)
{
	int		stop;
	char	*tmp;

	stop = 1;
	while (line[stop] && line[stop] != '\'')
		stop++;
	*index += stop + 1;
	tmp = ft_substr(line, 1, stop - 1);
	return (tmp);
}

char	*expand_double_quote(t_list *env, char *line, int *index)
{
	int				i;
	struct s_chars	chars;

	i = 1;
	chars.str = ft_strdup("");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (line[i] && line[i] != '"')
	{
		if (line[i] == '$')
			chars.expanded = expand_var(env, line + i, &i);
		else
			chars.expanded = ft_strdup((char [2]){line[i++], 0});
		if (!chars.expanded)
			return (perror("malloc"), NULL);
		chars.tmp = ft_strjoin(chars.str, chars.expanded);
		free(chars.expanded);
		free(chars.str);
		if (!chars.tmp)
			return (perror("malloc"), NULL);
		chars.str = chars.tmp;
	}
	*index += i;
	return (chars.str);
}
