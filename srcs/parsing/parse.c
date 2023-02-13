/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:01:02 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/13 18:26:18 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	init_cmd_and_proc(t_proc **proc, char **cmd, t_data *data, t_proc *last_proc)
{
	*proc = NULL;
	*cmd = ft_substr(*cmd, 0, next_pipeline(*cmd));
	if (!cmd)
		return (perror("malloc"), -1);
	*proc = create_and_push_proc(data, last_proc);
	if (!*proc)
	{
		free(*cmd);
		*cmd = NULL;
		return (perror("malloc"), -1);
	}
	return (0);
}

int	parse_command_or_subshell(t_data *data, char *line, t_proc *proc)
{
	char	*tmp;
	int		ret;

	line += ft_skip_spaces(line);
	if (!*line && proc->fd_out == STDOUT_FILENO && proc->fd_in == STDIN_FILENO)
		return (print_syntax_error("empty command", 0), 1);
	if (*line == '(')
	{
		proc->type = SUBSHELL;
		tmp = ft_substr(line, 1, skip_parenthesis(line));
		if (!tmp)
			return (perror("malloc"), -1);
		tmp[skip_parenthesis(line) - 2] = ' ';
		parse(data, tmp, proc);
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
int	parse(t_data *data, char *line, t_proc *last_proc)
{
	char			*cmd;
	t_proc			*proc;
	int				ret;

	if (check_unclosed(line))
		return (1);
	while (*line)
	{
		cmd = line;
		if (init_cmd_and_proc(&proc, &cmd, data, last_proc) < 0)
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
		last_proc = proc;
	}
	return (0);
}
