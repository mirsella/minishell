/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 15:38:38 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/12 16:55:54 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <string.h>

static int	ismeta(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '&');
}

char	*get_word_expand(t_data *data, char *line, int *ret)
{
	int		stop;
	char	*tmp;

	if (*(line + ft_skip_spaces(line)) == 0)
	{
		print_error("empty redirect", NULL);
		return (*ret = 1, NULL);
	}
	stop = 0;
	while (line[stop] && !ismeta(line[stop]) && !ft_isspace(line[stop]))
	{
		if (line[stop] == '*' || line[stop] == '(')
		{
			print_error("unexpected token ", (char [2]){*line, 0});
			return (*ret = 1, NULL);
		}
		if (line[stop] == '\'' || line[stop] == '"')
			stop += skip_quotes(line);
		else if (line[stop] == '(')
			stop += skip_parenthesis(line);
		else
			stop++;
	}
	tmp = ft_substr(line, 0, stop);
	line = expand_everything(data, tmp);
	free(tmp);
	return (*ret = 0, line);
}

int	remove_redirections(char *line)
{
	int	i;

	i = 0;
	i += ft_skip_spaces(line);
	while (line[i] == '>' || line[i] == '<')
		line[i++] = ' ';
	i += ft_skip_spaces(line + i);
	while (line[i] && !ismeta(line[i]) && !ft_isspace(line[i]))
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			ft_memset(line + i, ' ', skip_quotes(line + i));
			i += skip_quotes(line + i);
		}
		else if (line[i] == '(')
		{
			ft_memset(line + i, ' ', skip_parenthesis(line + i));
			i += skip_parenthesis(line + i);
		}
		else
			line[i++] = ' ';
	}
	return (i);
}

int	parse_redirections(t_data *data, char *line, t_proc *proc)
{
	int	ret;
	
	ret = 0;
	while (*line)
	{
		line += ft_skip_spaces(line);
		if (!*line)
			break ;
		if (*line == '>')
		{
			ret = output_redirection(data, line + 1, proc);
			line += remove_redirections(line);
		}
		// still need to parse input redirections here
		else
			line++;
		if (ret)
			return (ret);
	}
	return (0);
}
