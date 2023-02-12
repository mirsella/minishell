/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 18:10:11 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/12 01:13:18 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ismeta(char c)
{
	return (c == '|' || c == '&');
}

int	is_nextpipeline_possible(t_next_pipeline next_pipeline, char *line)
{
	if ((next_pipeline != INVALID && !*line)
			|| (next_pipeline == INVALID && *line))
	{
		print_syntax_error("near unexpected token ", *(line - 1));
		return (0);
	}
	return (1);
}

int	next_pipeline(char *line)
{
	int	i;
	int	ret;

	i = 0;
	while (line[i] && !ismeta(line[i]))
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
	if (*line == '|' && !ismeta(*(line + 1)))
		return (PIPE);
	if (ft_strncmp(line, "||", 2) == 0 && !ismeta(*(line + 2)))
		return (OR);
	if (ft_strncmp(line, "&&", 2) == 0 && !ismeta(*(line + 2)))
		return (AND);
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
