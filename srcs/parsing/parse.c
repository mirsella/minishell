/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:01:02 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/15 22:22:56 by mirsella         ###   ########.fr       */
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

int	init_cmd_and_proc(t_proc **first, t_proc **last_proc, t_proc **proc, char **cmd)
{
	*proc = NULL;
	*cmd = ft_substr(*cmd, 0, next_pipeline(*cmd));
	if (!cmd)
		return (perror("malloc"), -1);
	if (create_and_push_proc(first, last_proc, proc))
		return (free(*cmd), -1);
	return (0);
}

int	parse_command_or_subshell(char *line, t_list *env, t_proc **first, t_proc *proc)
{
	char	*tmp;
	int		ret;

	line += ft_skip_spaces(line);
	if (!*line && proc->fd_out == STDOUT_FILENO && proc->fd_in == STDIN_FILENO)
		return (print_syntax_error("no command given", 0), 1);
	ret = 0;
	if (*line == '(')
	{
		proc->type = SUBSHELL;
		tmp = ft_substr(line, 1, skip_parenthesis(line));
		if (!tmp)
			return (perror("malloc"), -1);
		tmp[skip_parenthesis(line) - 2] = ' ';
		ret = parse(tmp, env, first, proc);
		free(tmp);
	}
	else
	{
		// ret = parse_command(line, proc, env);
		proc->type = COMMAND;
		proc->line = ft_strdup(line);
		if (!proc->line)
			return (perror("malloc"), -1);
	}
	return (ret);
}

int	parse(char *line, t_list *env, t_proc **first, t_proc *last_proc)
{
	char			*cmd;
	t_proc			*proc;
	int				ret;

	if (check_unclosed(line)) // and should check for invalid pipeline like &&& or |\0. then we can remove is next_pipeline_possible
		return (1);
	while (*line)
	{
		cmd = line;
		if (init_cmd_and_proc(first, &last_proc, &proc, &cmd))
			return (-1);
		proc->next_pipeline = get_pipeline_type(line + next_pipeline(line));
		ret = parse_redirections(cmd, proc, env);
		if (ret)
			return (free(cmd), ret);
		ret = parse_command_or_subshell(cmd, env, first, proc);
		free(cmd);
		if (ret)
			return (ret);
		line += next_pipeline(line) + skip_pipeline(proc->next_pipeline);
		if (!is_nextpipeline_possible(proc->next_pipeline, line))
			return (1);
		last_proc = proc;
	}
	return (0);
}
