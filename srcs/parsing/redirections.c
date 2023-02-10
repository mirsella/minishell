/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 15:38:38 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/10 16:48:34 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <string.h>

// should expand quotes and variables, and return the first word
// char	*get_word(t_data *data, char *line)
// {
// }

int	handle_output_redirection(t_data *data, char *line, t_proc *proc)
{
	char	*filename;

	if (!(!ismeta(*line) || (*line == '>' && !ismeta(*(line + 1)))))
		return (print_syntax_error("invalid token ", *(line)), -1);
	(void)data;
	(void)proc;
	// filename = get_word(line);
	filename = ft_strdup(line);
	if (*line == '>')
		proc->fd_out = open(filename + 1, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		proc->fd_out = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (proc->fd_out < 0)
		return (print_error(filename, strerror(errno)), -1);
	free(filename);
	if (*line == '>')
		return (1);
	return (0);
}

// check for redirections: <, <<, >, >>. jump past (), "", ''. expand var and quotes which are after redir char. replace redir chars with spaces.
int	handle_redirections(t_data *data, char *line, t_proc *proc)
{
	int	ret;
	
	ret = 0;
	while (*line)
	{
		line += ft_skip_spaces(line);
		if (*line == '>')
			ret = handle_output_redirection(data, line + 1, proc);
		if (ret < 0)
			return (ret);
		line += ret + 1;
	}
	return (0);
}
