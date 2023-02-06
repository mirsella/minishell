/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:53:10 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/06 14:10:28 by lgillard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	line = NULL;
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
			break ;
		parse(line, envp);
	}
}
