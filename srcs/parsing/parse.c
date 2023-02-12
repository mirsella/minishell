/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:01:02 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/12 18:23:52 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	parse_command_or_subshell(t_data *data, char *line, t_proc *proc)
{
	char	*tmp;
	int		ret;

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
		ret = parse_command(data, line, proc);
		if (ret)
			return (ret);
	}
	return (0);
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
			return (free(cmd), ret);
		ret = parse_command_or_subshell(data, cmd, proc);
		if (ret)
			return (free(cmd), ret);
		free(cmd);
		line += next_pipeline(line) + skip_pipeline(proc->next_pipeline);
		if (!is_nextpipeline_possible(proc->next_pipeline, line))
			return (1);
	}
	return (0);
}
