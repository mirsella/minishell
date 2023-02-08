/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:01:02 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/08 19:04:15 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse(char *line, char **envp)
{
	(void)envp;
	printf("line: '%s'\n", line);
	while (*line)
	{
	}
	return (0);
}

/*
support for : >, >>, <, <<, |, &&, ||, (), ', "
parse routine:
1. check for redirections
2. check for pipes
3. check for && and ||
4. check for ()
5. check for $ variables
6. check for ' and "
7. check for cmd

*/
