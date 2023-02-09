/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 18:10:11 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/09 23:57:46 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	next_pipeline(char *line)
{
	int	i;
	int	ret;

	i = 0;
	while (line[i] && line[i] != '|' && line[i] != '&')
	{
		if (line[i] == '(')
		{
			ret = skip_parenthesis(line + i);
			if (ret == -1)
				return (-1);
			i += ret;
		}
		else if (line[i] == '"' || line[i] == '\'')
		{
			ret = skip_quotes(line + i);
			if (ret == -1)
				return (-1);
			i += ret;
		}
		else
			i++;
	}
	return (i);
}

t_next_pipeline	get_pipeline_type(char *line)
{
	if (*line == '|' && *(line + 1) != '|')
		return (PIPE);
	if (*line == '|' && *(line + 1) == '|')
	{
		if (*(line + 2) == '|')
			return (INVALID);
		else
			return (OR);
	}
	if (*line == '&' && *(line + 1) == '&')
	{
		if (*(line + 2) == '&')
			return (INVALID);
		else
			return (AND);
	}
	return (INVALID);
}

int	skip_pipeline(t_next_pipeline pipeline_type)
{
	if (pipeline_type == PIPE)
		return (1);
	else if (pipeline_type == AND || pipeline_type == OR)
		return (2);
	return (0);
}
