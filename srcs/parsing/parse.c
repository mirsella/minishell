/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:01:02 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/07 21:49:08 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse(char *line, char **envp)
{
	(void)envp;
	printf("line: '%s'\n", line);
	while (*line)
	{
		// first word can be: cmd, redirections, $ variables, 
	}
	return (0);
}
