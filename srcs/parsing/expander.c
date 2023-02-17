/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 23:42:59 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/17 17:41:49 by mirsella         ###   ########.fr       */
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
	return (c == '\'' || c == '"' || c == '$');
}

char	*expand_var(t_list *env, char *line, int *index)
{
	int		stop;
	char	*var;
	char	*tmp;

	stop = 1;
	if (line[1] == '?')
		stop++;
	else
		while (line[stop] && !ismeta(line[stop]) && !isspace(line[stop]))
			stop++;
	*index += stop;
	if (stop == 1)
		return (ft_strdup("$"));
	tmp = ft_substr(line, 1, stop - 1);
	var = get_env_var(env, tmp);
	free(tmp);
	if (!var)
		return (ft_strdup(""));
	return (ft_strdup(var));
}

char	*get_in_quote(char *line, int *index)
{
	int		stop;
	char	quote;
	char	*tmp;

	quote = line[0];
	stop = 1;
	while (line[stop] && line[stop] != quote)
		stop++;
	*index += stop + 1;
	tmp = ft_substr(line, 1, stop - 1);
	return (tmp);
}
