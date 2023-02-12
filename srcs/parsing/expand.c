/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 23:42:59 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/12 20:35:45 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ismeta(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '&'
		|| c == '(' || c == '\'' || c == '"' || c == '$');
}

char	*expand_var(t_list *env, char *str, int *index)
{
	int		stop;
	char	*var;
	char	*tmp;

	stop = 1;
	while (str[stop] && !ismeta(str[stop]) && !isspace(str[stop]))
		stop++;
	if (stop == 1)
		return ((*index)++, ft_strdup("$"));
	*index += stop;
	tmp = ft_substr(str, 1, stop - 1);
	var = get_env_value(env, tmp);
	free(tmp);
	if (var == NULL)
		return (ft_strdup(""));
	return (ft_strdup(var));
}

char	*expand_single_quote(char *str, int *index)
{
	int		stop;
	char	*tmp;

	stop = 1;
	while (str[stop] && str[stop] != '\'')
		stop++;
	*index += stop + 1;
	tmp = ft_substr(str, 1, stop - 1);
	return (tmp);
}

struct s_chars
{
	char	*tmp;
	char	*str;
	char	*expanded;
};

char	*expand_double_quote(t_list *env, char *str, int *index)
{
	int				i;
	struct s_chars	chars;

	i = 1;
	chars.str = ft_strdup("");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (str[i] && str[i] != '"')
	{
		if (str[i] == '$')
			chars.expanded = expand_var(env, str + i, &i);
		else
			chars.expanded = ft_strdup((char [2]){str[i++], 0});
		if (!chars.expanded)
			return (perror("malloc"), NULL);
		chars.tmp = ft_strjoin(chars.str, chars.expanded);
		free(chars.expanded);
		free(chars.str);
		if (!chars.tmp)
			return (perror("malloc"), NULL);
		chars.str = chars.tmp;
	}
	*index += i + 1;
	return (chars.str);
}

char	*expand_everything(t_list *env, char *str)
{
	int				i;
	struct s_chars	chars;

	i = 0;
	chars.str = ft_strdup("");
	if (!chars.str)
		return (perror("malloc"), NULL);
	while (str[i])
	{
		if (str[i] == '\'')
			chars.expanded = expand_single_quote(str + i, &i);
		else if (str[i] == '"')
			chars.expanded = expand_double_quote(env, str + i, &i);
		else if (str[i] == '$')
			chars.expanded = expand_var(env, str + i, &i);
		else
			chars.expanded = ft_strdup((char [2]){str[i++], 0});
		if (!chars.expanded)
			return (perror("malloc"), NULL);
		chars.tmp = ft_strjoin(chars.str, chars.expanded);
		free(chars.expanded);
		free(chars.str);
		chars.str = chars.tmp;
	}
	return (chars.str);
}
