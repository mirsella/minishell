/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:53:10 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/15 23:44:26 by mirsella         ###   ########.fr       */
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
	if (*envp)
	{
		env_tab = ft_tabdup(envp);
		if (!env)
			return (perror("malloc"), -1);
		*env = ft_lstnew_strs(ft_tablen(env_tab), env_tab);
		free(env_tab);
		if (!(*env))
			return (perror("malloc"), -1);
	}
	else
		*env = NULL;
	return (0);
}

// only add to history if the line doesn't start with a space
void	add_history_filter(char *line)
{
	if (!ft_isspace(*line))
		add_history(line);
}

int	handle_line(char *line, t_list *env)
{
	int		ret;
	t_proc	*procs;

	procs = NULL;
	ret = parse(line, env, &procs, NULL);
	if (ret)
		return (procs_free(&procs), ret);
	ret = execute(procs, env);
	procs_free(&procs);
	return (ret);
}

int	prompt_loop(t_list *env)
{
	char	*line;
	int		ret;

	line = NULL;
	while (1)
	{
		free(line);
		line = readline(PROMPT);
		if (!line)
			break ;
		if (!*(line + ft_skip_spaces(line)))
			continue ;
		add_history_filter(line);
		ret = handle_line(line, env);
		if (ret < 0)
			break ;
		else if (ret > 1)
			continue ;
	}
	free(line);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*env;

	(void)argc;
	(void)argv;
	if (init_shell(&env, envp))
		exit_shell(env);
	prompt_loop(env);
	exit_shell(env);
}
