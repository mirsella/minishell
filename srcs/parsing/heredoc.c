/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 13:51:48 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/28 14:11:56 by lgillard         ###   ########.fr       */
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

void	sigint_heredoc_handler(int sig)
{
	(void)sig;
	g_exit_code = 128 + SIGINT;
	// ft_putstr("\n> ");
}
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

// int	prompt_loop(t_list *env)
// {
// 	char	*line;
// 	int		ret;

// 	line = NULL;
// 	while (1)
// 	{
// 		line = readline();
// 		if (!line)
// 			break ;
// 		if (!*(line + ft_skip_spaces(line)))
// 			continue ;
// 		if (!ft_isspace(*line))
// 			add_history(line);
// 		ret = handle_line(line, env);
// 		if (ret < 0)
// 			break ;
// 		if (ret > 1)
// 			continue ;
// 	}
// 	free(line);
// 	return (0);
// }

int	get_line(char *delim, char **str)
{
	char	*line;
	char	*tmp;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sigint_heredoc_handler);
	rl_getc_function = getc;
	line = readline("> ");
	if (!line)
	{
		if (g_exit_code == 128 + SIGINT)
			return (1);
		printf("minishell: warning: %s wanted `%s'\n",
			"here-document delimited by end-of-file", delim);
		return (0);
	}
	tmp = ft_substr(line, 0, ft_strlen(line));
	free(line);
	if (!tmp)
		return (-1);
	*str = tmp;
	return (0);
}

int	read_until_delim(char *delim, int expand, int fd, t_list *env)
{
	char	*line;
	char	*tmp;
	int		ret;

	line = "";
	while (line)
	{
		ret = get_line(delim, &line);
		if (ret)
			return (1);
		if (ft_strcmp(line, delim) == 0)
			return (free(line), 0);
		if (expand)
			tmp = expand_vars(line, env);
		else
			tmp = ft_strdup(line);
		free(line);
		if (!tmp)
			return (-1);
		ft_putendl_fd(tmp, fd);
		free(tmp);
	}
	return (0);
}



int	heredoc_redirection(char *line, t_proc *proc, t_list *env)
{
	int		pipes[2];
	char	*delim;
	int		ret;
	int		expand;

	ret = 0;
	ret = set_heredoc_delim(line, &delim, &expand);
	if (ret)
		return (ret);
	if (pipe(pipes) == -1)
		return (-1);
	proc->fd_in = pipes[0];
	ret = read_until_delim(delim, expand, pipes[1], env);
	free(delim);
	close(pipes[1]);
	proc->fd_in = pipes[0];
	return (ret);
}
