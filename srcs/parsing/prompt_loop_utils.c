/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_loop_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 23:14:50 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/10 00:28:15 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// only add to history if the line doesn't start with a space
void	add_history_filter(char *line)
{
	if (!ft_isspace(*line))
		add_history(line);
}

int	check_unclosed_parenthesis(char *line)
{
	int	ret;

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
