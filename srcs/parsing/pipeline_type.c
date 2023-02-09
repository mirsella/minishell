/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 18:10:11 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/09 20:45:12 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	next_pipeline(char *line)
{
	int		i;

	i = 0;
	while (line[i] && line[i] != '|' && line[i] != '&')
	{
		if (line[i] == '(')
			i += skip_parentheses(line + i);
		else if (line[i] == '"' || line[i] == '\'')
			i += skip_quotes(line + i);
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
