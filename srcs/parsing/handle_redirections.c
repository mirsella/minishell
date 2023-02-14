/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 22:31:48 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/14 11:36:30 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ismeta(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '&' || c == '(');
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

int	output_redirection(t_data *data, char *line, t_proc *proc)
{
	char	*filename;
	int		append;
	int		ret;

	append = 0;
	if (*line == '>')
		append = 1;
	line += append;
	line += ft_skip_spaces(line + append);
	if (ismeta(*line))
		return (print_syntax_error("unexpected token ", *(line)), 1);
	filename = get_word_expand(data, line, &ret);
	if (ret)
		return (ret);
	if (!filename)
		return (-1);
	proc->fd_out = open_writing_file(filename, proc, append);
	if (proc->fd_out == -1)
		return (1);
	free(filename);
	return (0);
}
