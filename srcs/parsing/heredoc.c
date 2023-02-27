/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 13:51:48 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/27 19:50:03 by mirsella         ###   ########.fr       */
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

int	set_heredoc_delim(char *line, char **delim, int *expand)
{
	struct s_chars	chars;

	*expand = 1;
	chars.str = NULL;
	while (*line && !ft_strchr("<>|&", *line) && !isspace(*line))
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

int	prompt_null_line(char *delim)
{
	if (g_exit_code == 128 + SIGINT)
		return (1);
	printf("minishell: warning: %s. wanted `%s'\n",
		"here-document delimited by end-of-file (wanted `", delim);
	return (0);
}

char	*get_line(char *delim)
{
	char	*line;
	char	*tmp;

	ft_putstr("> ");
	line = ft_get_next_line(STDIN_FILENO, 0);
	if (!line)
		return (prompt_null_line(delim), NULL);
	tmp = ft_substr(line, 0, ft_strlen(line) - 1);
	free(line);
	if (!tmp)
		return (NULL);
	return (tmp);
}

int	read_until_delim(char *delim, int expand, int fd, t_list *env)
{
	char	*line;
	char	*tmp;

	line = "";
	while (line)
	{
		line = get_line(delim);
		if (!line)
			return (-1);
		if (ft_strcmp(line, delim) == 0)
			return (free(line), ft_get_next_line(STDIN_FILENO, 1), 0);
		if (expand)
			tmp = expand_vars(line, env);
		else
			tmp = ft_strdup(line);
		free(line);
		if (!tmp)
			return (ft_get_next_line(STDIN_FILENO, 1), -1);
		ft_putendl_fd(tmp, fd);
		free(tmp);
	}
	ft_get_next_line(STDIN_FILENO, 1);
	return (0);
}

int	heredoc_redirection(char *line, t_proc *proc, t_list *env)
{
	int		pipes[2];
	char	*delim;
	int		ret;
	int		expand;

	signal(SIGQUIT, SIG_IGN);
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
	return (0);
}
