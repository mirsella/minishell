/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_unclosed_and_invalid_pipeline.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 23:14:50 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/17 18:41:29 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_unclosed_parenthesis(char *line)
{
	int	ret;

	ret = 0;
	if (*line == '(')
	{
		ret = skip_parenthesis(line);
		if (ret == -1)
			return (print_syntax_error("unclosed parenthesis ", *line), ret);
		else if (ret == -2)
			return (print_syntax_error("unclosed quotes ", *line), ret);
	}
	else if (*line == ')')
		return (print_syntax_error("unexpected closing parenthesis ",
				*line), -3);
	return (ret);
}

int	check_unclosed(char *line)
{
	int	ret;

	while (*line)
	{
		ret = check_unclosed_parenthesis(line);
		if (ret < 0)
			return (ret);
		else if (ret > 0)
			line += ret;
		else if (*line == '\'' || *line == '"')
		{
			ret = skip_quotes(line);
			if (ret == -1)
				return (print_syntax_error("unclosed quotes ", *line), 1);
			line += ret;
		}
		else
			line++;
	}
	return (0);
}

static int	ismeta(char c)
{
	return (c == '|' || c == '&');
}

int	skip_token(char *line)
{
	int	i;

	i = 0;
	if (line[i] == '(')
		i += skip_parenthesis(line + i);
	else if (line[i] == '"' || line[i] == '\'')
		i += skip_quotes(line + i);
	else
		i++;
	return (i);
}

int	check_unclosed_and_invalid_pipeline(char *line)
{
	int				seen_word;
	t_next_pipeline	type;

	if (check_unclosed(line))
		return (1);
	while (*line)
	{
		seen_word = 0;
		while (*line && !ismeta(*line))
		{
			if (!ft_isspace(*line))
				seen_word = 1;
			line += skip_token(line);
		}
		if (!seen_word && (ismeta(*line) || *line == 0))
			return (print_syntax_error("near unexpected token ", *line), 1);
		if (!*line)
			break ;
		type = get_pipeline_type(line);
		if (type == INVALID || (*(line + skip_pipeline(type)) == 0))
			return (print_syntax_error("near unexpected token ", *line), 1);
		line += skip_pipeline(type);
	}
	return (0);
}
