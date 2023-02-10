/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:01:02 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/10 16:37:11 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ismeta(char c)
{
	return (c == '|' || c == '<' || c == '&' || c == '('
			|| c == ')' || c == '>');
}

int	handle_command_or_subshell(t_data *data, char *line, t_proc *proc)
{
	char			*tmp;

	line += ft_skip_spaces(line);
	if (*line == '(')
	{
		proc->type = SUBSHELL;
		tmp = ft_substr(line, 1, skip_parenthesis(line));
		if (!tmp)
			return (-1);
		tmp[skip_parenthesis(line) - 2] = ' ';
		parse(data, tmp, proc);
		free(tmp);
	}
	else
	{
		// expand variables and wildcard for first word
		proc->type = COMMAND;
		proc->path = ft_strdup(line); // should search for file in $PATH or builtin
		if (!proc->path)
			return (-1);
	}
	return (0);
}

int	parse(t_data *data, char *line, t_proc *last_proc)
{
	char			*cmd;
	t_proc			*proc;

	while (*line)
	{
		cmd = ft_substr(line, 0, next_pipeline(line));
		proc = create_and_push_proc(data, last_proc);
		if (!cmd || !proc)
			return (perror("malloc"), -1);
		proc->next_pipeline = get_pipeline_type(line + next_pipeline(line));
		if (handle_redirections(data, cmd, proc))
			return (1);
		if (handle_command_or_subshell(data, cmd, proc) == -1)
			return (perror("malloc"), -1);
		free(cmd);
		line += next_pipeline(line) + skip_pipeline(proc->next_pipeline);
		if ((proc->next_pipeline != INVALID && !*line)
			|| (proc->next_pipeline == INVALID && *line))
			return (print_error_char("syntax error near unexpected token ",
					*(line - 1)), 1);
		last_proc = proc;
	}
	return (0);
}
