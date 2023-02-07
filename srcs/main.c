/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:53:10 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/07 23:59:50 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

int	g_exit_code;

int	init_shell(t_data *data, char **envp)
{
	char	**env;

	g_exit_code = 0;
	call_sigaction();
	env = ft_tabdup(envp);
	if (!env)
		return (perror("malloc"), 1);
	data->env = ft_lstnew_strs(ft_tablen(env), env);
	free(env);
	if (!data->env)
		return (perror("malloc"), 1);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	char	*line;

	(void)argc;
	(void)argv;
	bzero(&data, sizeof(t_data));
	if (init_shell(&data, envp))
		return (1);
	line = NULL;
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			ft_putstr_fd("exit", STDERR_FILENO);
			break ;
		}
		pass_spaces(line);
		if (!*line)
			break ;
		add_history(line);
		parse(line, envp);
		free(line);
	}
	exit_shell(&data);
}
