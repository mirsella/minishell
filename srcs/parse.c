/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:01:02 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/06 14:07:56 by lgillard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse(char *line, char **envp)
{
	int	i;

	i = 0;
	if (line[0] == '\0')
		return (1);
	printf("line: %s\n", line);
	while (i < 10 && envp[i])
	{
		printf("%s\n", envp[i]);
	}
	return (0);
}
