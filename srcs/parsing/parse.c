/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:01:02 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/09 23:25:47 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_proc	*create_and_push_proc(t_data *data, t_proc *last_proc)
{
	t_proc	*new;

	new = new_proc();
	if (!new)
		return (NULL);
	if (!last_proc)
		data->procs = new;
	else if (last_proc->type == COMMAND
		|| (last_proc->type == SUBSHELL && last_proc->procs))
		last_proc->next = new;
	else if (last_proc->type == SUBSHELL && !last_proc->procs)
		last_proc->procs = new;
	return (new);
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
		// check for redirections: <, <<, >, >>. jump past (), "", ''. stop at |, &&, ||, \0. expand var  and quotes which are after redir char. replace redir chars with spaces.
		if (handle_command_or_subshell(data, cmd, proc) == -1)
			return (perror("malloc"), -1);
		free(cmd);
		line += next_pipeline(line) + skip_pipeline(proc->next_pipeline);
		if ((proc->next_pipeline != INVALID && !*line)
			|| (proc->next_pipeline == INVALID && *line))
			return (print_error("syntax error near unexpected token ",
					*(line - 1)), 1);
		last_proc = proc;
	}
	return (0);
}
