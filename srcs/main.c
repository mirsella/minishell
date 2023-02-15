/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:53:10 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/15 22:22:02 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>
#include <unistd.h>

int	g_exit_code;

int	init_shell(t_list **env, char **envp)
{
	char	**env_tab;

	g_exit_code = 0;
	call_sigaction();
	env_tab = ft_tabdup(envp);
	if (!env)
		return (perror("malloc"), -1);
	*env = ft_lstnew_strs(ft_tablen(env_tab), env_tab);
	free(env_tab);
	if (!(*env))
		return (perror("malloc"), -1);
	return (0);
}

int	prompt_loop(t_list *env)
{
	char	*line;
	int		ret;
	t_proc	*procs;

	procs = NULL;
	line = NULL;
	while (1)
	{
		free(line);
		procs_free(&procs);
		line = readline(PROMPT);
		if (!line)
			break ;
		if (!*(line + ft_skip_spaces(line)))
			continue ;
		add_history_filter(line);
		ret = parse(line, env, &procs, NULL);
		if (ret < 0)
			break ;
		else if (ret > 0)
			continue ;
		ret = execute(procs, env);
		if (ret < 0)
			break ;
		else if (ret > 0)
			continue ;
	}
	free(line);
	procs_free(&procs);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_list *env;

	(void)argc;
	(void)argv;
	if (init_shell(&env, envp))
		exit_shell(env);
	prompt_loop(env);
	exit_shell(env);
}
