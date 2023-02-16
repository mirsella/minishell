/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 22:31:48 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/17 00:32:10 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ismeta(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '&' || c == '(');
}

int	open_reading_file(char *filename, t_proc *proc)
{
	if (proc->fd_in >= 3)
		close(proc->fd_in);
	proc->fd_in = open(filename, O_RDONLY);
	if (proc->fd_in < 0)
		print_error(filename, strerror(errno));
	return (0);
}

int	open_writing_file(char *filename, t_proc *proc, int append_flag)
{
	if (proc->fd_out >= 3)
		close(proc->fd_out);
	if (append_flag)
		proc->fd_out = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		proc->fd_out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (proc->fd_out < 0)
		print_error(filename, strerror(errno));
	return (0);
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
	ret = open_writing_file(filename, proc, append);
	free(filename);
	return (ret);
}

int	input_redirection(char *line, t_proc *proc, t_list *env)
{
	char	*filename;
	int		heredoc;
	int		ret;

	heredoc = 0;
	ret = 0;
	if (*line == '<')
		heredoc = 1;
	line += heredoc;
	line += ft_skip_spaces(line);
	if (ismeta(*line) || *line == 0)
		return (print_syntax_error("unexpected token near empty redirect ",
				*(line)), 1);
	if (heredoc)
		ret = heredoc_redirection(line, proc, env);
	else
	{
		filename = get_redirect_word_expand(line, &ret, env);
		if (ret)
			return (ret);
		ret = open_reading_file(filename, proc);
		free(filename);
	}
	return (ret);
}
