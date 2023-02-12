/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 23:42:59 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/12 16:50:21 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ismeta(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '&'
		|| c == '(' || c == '\'' || c == '"' || c == '$');
}

char	*expand_var(t_data *data, char *str, int *index)
{
	int		stop;
	char	*var;
	char	*tmp;

	stop = 1;
	// *index += 1;
	while (str[stop] && !ismeta(str[stop]) && !isspace(str[stop]))
		stop++;
	if (stop == 1)
		return ((*index)++, ft_strdup("$"));
	*index += stop;
	tmp = ft_substr(str, 1, stop);
	var = get_env_value(data->env, tmp);
	free(tmp);
	if (var == NULL)
		return (ft_strdup(""));
	printf("returning %s\n", var);
	return (ft_strdup(var));
}

char	*expand_single_quote(char *str, int *index)
{
	int		stop;
	char	*tmp;

	stop = 1;
	// *index += 1;
	while (str[stop] && str[stop] != '\'')
		stop++;
	if (str[stop] != '\'')
		return (NULL);
	*index += stop;
	tmp = ft_substr(str, 0, stop);
	return (tmp);
}

char	*expand_double_quote(t_data *data, char *str, int *index)
{
	int		stop;
	char	*tmp;
	char	*new;

	stop = 1;
	// *index += 1;
	while (str[stop] && str[stop] != '"')
	{
			tmp = new;
			if (str[stop] == '$')
				new = ft_strjoin(tmp, expand_var(data, str + stop, &stop));
			else
				new = ft_strjoin(tmp, (char [2]){str[stop], 0});
			free(tmp);
	}
	if (str[stop] != '"')
		return (NULL);
	*index += stop;
	return (new);
}

char	*expand_everything(t_data *data, char *str)
{
	int		i;
	char	*new;
	char	*tmp;

	i = 0;
	new = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '\'')
			tmp = expand_single_quote(str + i, &i);
		else if (str[i] == '"')
			tmp = expand_double_quote(data, str + i, &i);
		else if (str[i] == '$')
			tmp = expand_var(data, str + i, &i);
		else
			tmp = ft_strjoin(new, (char [2]){str[i++], 0});
		if (!tmp)
			return (free(new), NULL);
		free(new);
		new = tmp;
	}
	return (new);
}
