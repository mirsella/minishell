/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skipping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 17:30:13 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/10 00:17:26 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	skip_quotes(char *line)
{
	int		i;
	char	quote;

	i = 1;
	quote = line[0];
	while (line[i] && line[i] != quote)
			i++;
	if (line[i] == quote)
		return (i + 1);
	return (-1);
}

int	skip_parenthesis(char *line)
{
	int	i;
	int	ret;

	i = 1;
	while (line[i] && line[i] != ')')
	{
		if (line[i] == '(')
		{
			ret = skip_parenthesis(line + i);
			if (ret == -1)
				return (-1);
			i += ret - 1;
		}
		else if (line[i] == '"' || line[i] == '\'')
		{
			ret = skip_quotes(line + i);
			if (ret == -1)
				return (-2);
			i += ret - 1;
		}
		i++;
	}
	if (line[i] == ')')
		return (i + 1);
	return (-1);
}
