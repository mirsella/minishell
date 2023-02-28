/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 23:42:59 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/28 15:21:53 by lgillard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

struct s_chars
{
	char	*tmp;
	char	*str;
	char	*expanded;
};

static int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	get_identifier_stop(char *line)
{
	int		stop;
	char	*tmp;

	stop = 1;
	tmp = ft_substr(line, 1, stop);
	if (!tmp)
		return (-1);
	while (is_valid_identifier(tmp) && line[stop])
	{
		stop++;
		free(tmp);
		tmp = ft_substr(line, 1, stop);
		if (!tmp)
			return (-1);
	}
	free(tmp);
	return (stop - 1);
}

char	*expand_var(t_list *env, char *line, int *index)
{
	int		stop;
	char	*var;
	char	*tmp;

	stop = 1;
	if (line[1] == '?')
	{
		*index += 2;
		return (ft_itoa(g_exit_code));
	}
	stop += get_identifier_stop(line);
	if (stop == -1)
		return (NULL);
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

char	*expand_quote(char *line, int *index, t_list *env)
{
	char	*tmp;

	tmp = NULL;
	if (*line == '"')
		tmp = expand_vars_in_double_quote(line, index, env);
	else if (*line == '\'')
	{
		tmp = ft_substr(line, 0, skip_quotes(line));
		*index += skip_quotes(line);
	}
	return (tmp);
}
