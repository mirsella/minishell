/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 23:42:59 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/12 00:56:09 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ismeta(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '&'
		|| c == '(' || c == '\'' || c == '"');
}

char	*expand_var(t_data *data, char *str, int *index)
{
	int		stop;
	char	*var;
	char	*tmp;

	stop = 0;
	*index += 1;
	while (str[stop] && ismeta(str[stop]))
		stop++;
	if (stop == 0)
		return (NULL);
	*index += stop;
	tmp = ft_substr(str, 0, stop);
	var = get_env_value(data->env, tmp);
	free(tmp);
	if (var == NULL)
		return ("");
	return (var);
}

char	*expand_single_quote(t_data *data, char *str, int *index);
char	*expand_double_quote(t_data *data, char *str, int *index);
char	*expand_wildcard(t_data *data, char *str, int *index);

char	*expand_everything(t_data *data, char *str)
{
	(void)data;
	(void)index;
	return (ft_strdup(str));
}
