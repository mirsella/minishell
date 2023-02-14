/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:01:02 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/14 22:37:09 by mirsella         ###   ########.fr       */
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

int	init_cmd_and_proc(t_proc **proc, char **cmd,
	t_data *data, t_proc *last_proc)
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
		handle_line(data, tmp, proc);
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

// while line and next_pipeline == PIPE (if it's && or || we return))
//   parse each pipeline
//   skip the | and continue the loop
int	parse(t_data *data, char *line, t_proc *last_proc);
