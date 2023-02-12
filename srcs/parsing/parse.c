/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:01:02 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/12 01:19:23 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_command_or_subshell(t_data *data, char *line, t_proc *proc)
{
	char			*tmp;

	line += ft_skip_spaces(line);
	if (*line == '(')
	{
		proc->type = SUBSHELL;
		tmp = ft_substr(line, 1, skip_parenthesis(line));
		if (!tmp)
			return (perror("malloc"), -1);
		tmp[skip_parenthesis(line) - 2] = ' ';
		parse(data, tmp);
		free(tmp);
	}
	else
	{
		// expand variables and wildcard for first word
		proc->type = COMMAND;
		proc->path = ft_strdup(line); // should search for file in $PATH or builtin
		if (!proc->path)
			return (perror("malloc"), -1);
	}
	return (0);
}

void	init_cmd_and_proc(t_proc **proc, char **cmd, t_data *data, char *line)
{
	t_proc	*last_proc;

	last_proc = NULL;
	*cmd = NULL;
	*proc = NULL;
	*cmd = ft_substr(line, 0, next_pipeline(line));
	if (!cmd)
	{
		perror("malloc");
		return ;
	}
	if (data->procs)
		last_proc = get_last_proc(data->procs);
	*proc = create_and_push_proc(data, last_proc);
	if (!*proc)
	{
		free(*cmd);
		*cmd = NULL;
		perror("malloc");
		return ;
	}
}

// return > 0 mean parsing error, show new prompt
// return < 0 mean fatal error, exit
int	parse(t_data *data, char *line)
{
	char			*cmd;
	t_proc			*proc;
	int				ret;

	if (check_unclosed(line))
		return (1);
	while (*line)
	{
		init_cmd_and_proc(&proc, &cmd, data, line);
		if (!proc || !cmd)
			return (-1);
		proc->next_pipeline = get_pipeline_type(line + next_pipeline(line));
		ret = parse_redirections(data, cmd, proc);
		if (ret)
			return (ret);
		ret = parse_command_or_subshell(data, cmd, proc);
		if (ret)
			return (ret);
		free(cmd);
		line += next_pipeline(line) + skip_pipeline(proc->next_pipeline);
		if (!is_nextpipeline_possible(proc->next_pipeline, line))
			return (1);
	}
	return (0);
}
