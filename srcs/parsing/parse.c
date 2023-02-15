/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:01:02 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/15 19:35:04 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_next_token(char *line, int *index)
{
	char	*token;
	int		i;

	i = 0;
	while (line[i] && !ft_isspace(line[i]))
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			i += skip_quotes(line + i);
		}
		else
			i++;
	}
	token = ft_substr(line, 0, i);
	if (!token)
		return (perror("malloc"), NULL);
	*index += i;
	return (token);
}

char	*get_pipelines(char *line, int *index, t_next_pipeline *last_pipeline_type)
{
	int				i;
	char			*cmd;
	t_next_pipeline	next_pipeline_type;

	i = 0;
	while (line[i])
	{
		i += next_pipeline(line + i);
		next_pipeline_type = get_pipeline_type(line + i);
		if (next_pipeline_type == OR || next_pipeline_type == AND)
			break ;
		i += skip_pipeline(next_pipeline_type);

	}
	cmd = ft_substr(line, 0, i);
	if (!cmd)
		return (perror("malloc"), NULL);
	*last_pipeline_type = next_pipeline_type;
	*index += i;
	return (cmd);
}

int	init_cmd_and_proc(t_proc **last_proc, t_proc **proc, char **cmd)
{
	*cmd = ft_substr(*cmd, 0, next_pipeline(*cmd));
	if (!cmd)
		return (perror("malloc"), -1);
	create_and_push_proc(last_proc, proc);
	if (!proc)
	{
		free(*cmd);
		*cmd = NULL;
		return (perror("malloc"), -1);
	}
	return (0);
}

// ((ls && pwd) | grep mot) && cat doc
// ((ls && pwd) && grep mot) && cat doc
// (false) && true
// (true) && true
// (true) && false
// (cat doc | grep mot) && cat doc
// ls | (grep a)
// ls | (grep a && pwd)
int	parse_command_or_subshell(char *line, t_proc *proc, t_list *env)
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
		ret = handle_line(tmp, env);
		free(tmp);
	}
	else
	{
		proc->type = COMMAND;
		ret = parse_command(line, proc, env);
	}
	return (ret);
}

int	parse(char *line, t_proc **procs, t_list *env)
{
	char		*cmd;
	t_proc		**last_proc;
	t_proc		*proc;
	int			ret;

	last_proc = procs;
	while (*line)
	{
		cmd = line;
		if (init_cmd_and_proc(last_proc, &proc, &cmd) < 0)
			return (-1);
		proc->next_pipeline = get_pipeline_type(line + next_pipeline(line));
		ret = parse_redirections(cmd, proc, env);
		if (ret)
			return (free(cmd), ret);
		ret = parse_command_or_subshell(cmd, proc, env);
		if (ret)
			return (free(cmd), ret);
		free(cmd);
		line += next_pipeline(line) + skip_pipeline(proc->next_pipeline);
	}
	return (0);
}
