/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_history_filter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 17:24:03 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/06 17:43:37 by lgillard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_history_filter(char *line)
{
	if (!line)
		return ;
	printf("line before pass_spaces: '%s'\n", line);
	line = pass_spaces(line);
	printf("line after pass_spaces: '%s'\n", line);
	if (!*line)
		return ;
	add_history(line);
}
