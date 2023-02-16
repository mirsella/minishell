/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 13:51:48 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/17 00:32:20 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct	s_chars {
	char	*tmp;
	char	*str;
	char	*joined;
};

static int	ismeta(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '&');
}

int	set_heredoc_delim(char *line, char **delim, int *expand)
{
	struct s_chars	chars;

	*expand = 1;
	chars.str = NULL;
	while (*line && !ismeta(*line) && !isspace(*line))
	{
		if (*line == '\'' || *line == '"')
		{
			*expand = 0;
			chars.tmp = ft_substr(line, 1, skip_quotes(line) - 2);
			line += skip_quotes(line);
		}
		else
			chars.tmp = ft_substr(line++, 0, 1);
		if (!chars.tmp)
			return (perror("malloc"), -1);
		chars.joined = ft_strjoin_free(chars.str, chars.tmp);
		if (!chars.joined)
			return (perror("malloc"), -1);
		chars.str = chars.joined;
	}
	*delim = chars.str;
	return (0);
}

int	read_until_delim(char *delim, int expand, int fd, t_list *env)
{
	char	*line;
	char	*tmp;

	line = "";
	while (line)
	{
		line = readline("> ");
		if (!line)
			return (printf("minishell: warning: %s. wanted `%s'\n",
					"here-document delimited by end-of-line", delim), 0);
		if (ft_strcmp(line, delim) == 0)
			return (free(line), 0);
		if (expand)
			tmp = expand_vars(line, env);
		else
			tmp = ft_strdup(line);
		free(line);
		if (!tmp)
			return (-1);
		ft_putstr_fd(tmp, fd);
		ft_putstr_fd("\n", fd);
		free(tmp);
	}
	return (0);
}

void	print_fd(int fd)
{
	char	buf[1024];
	int		ret;

	ret = 1;
	printf("printing heredoc at fd %d\n", fd);
	printf("you should disable this function if you want the fd to not be empty for the execution\n");
	while (ret)
	{
		ret = read(fd, buf, 1024);
		write(1, buf, ret);
	}
}

int	heredoc_redirection(char *line, t_proc *proc, t_list *env)
{
	int		pipes[2];
	char	*delim;
	int		ret;
	int		expand;

	ret = set_heredoc_delim(line, &delim, &expand);
	if (ret)
		return (ret);
	if (pipe(pipes) == -1)
		return (-1);
	proc->fd_in = pipes[0];
	read_until_delim(delim, expand, pipes[1], env);
	free(delim);
	close(pipes[1]);
	proc->fd_in = pipes[0];
	print_fd(pipes[0]);
	return (0);
}
