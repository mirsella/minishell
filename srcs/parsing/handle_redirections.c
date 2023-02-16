/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 22:31:48 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/16 14:08:32 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ismeta(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '&' || c == '(');
}

int	open_reading_file(char *filename, t_proc *proc)
{
	int	fd;

	if (proc->fd_in >= 3)
		close(proc->fd_in);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		print_error(filename, strerror(errno));
		return (-1);
	}
	return (fd);
}

int	open_writing_file(char *filename, t_proc *proc, int append_flag)
{
	int	fd;

	if (proc->fd_out >= 3)
		close(proc->fd_out);
	if (append_flag)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		print_error(filename, strerror(errno));
		return (-1);
	}
	return (fd);
}

int	output_redirection(char *line, t_proc *proc, t_list *env)
{
	char	*filename;
	int		append;
	int		ret;

	append = 0;
	if (*line == '>')
		append = 1;
	line += append;
	line += ft_skip_spaces(line);
	if (ismeta(*line))
		return (print_syntax_error("unexpected token ", *(line)), 1);
	filename = get_redirect_word_expand(line, &ret, env);
	if (ret)
		return (ret);
	proc->fd_out = open_writing_file(filename, proc, append);
	if (proc->fd_out == -1)
		return (1);
	free(filename);
	return (0);
}

int	input_redirection(char *line, t_proc *proc, t_list *env)
{
	char	*filename;
	int		heredoc;
	int		ret;

	heredoc = 0;
	if (*line == '<')
		heredoc = 1;
	line += ft_skip_spaces(line);
	if (ismeta(*line))
		return (print_syntax_error("unexpected token ", *(line)), 1);
	if (heredoc)
	{
		printf("heredoc!\n");
		// proc->fd_in = heredoc_redirection(line, proc, env);
		// if (proc->fd_in == -1)
		// 	return (-1);
		return (0);
	}
	filename = get_redirect_word_expand(line, &ret, env);
	if (ret)
		return (ret);
	proc->fd_in = open_reading_file(filename, proc);
	free(filename);
	if (proc->fd_in == -1)
		return (1);
	return (0);
}
