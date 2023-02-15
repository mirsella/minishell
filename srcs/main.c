/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:53:10 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/15 19:36:24 by mirsella         ###   ########.fr       */
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

int	handle_line(char *line, t_list *env)
{
	char	*cmd;
	int		i;
	int		ret;
	t_proc	*procs;
	t_next_pipeline	next_pipeline_type;

	if (check_unclosed(line)) // shoud also check for syntax error like & or ||| or &&& or |\0
		return (1);
	procs = NULL;
	i = 0;
	while (line[i])
	{
		cmd = get_pipelines(line + i, &i, &next_pipeline_type);
		if (!cmd)
			return (-1);
		ret = parse(cmd, &procs, env);
		free(cmd);
		if (ret)
			return (procs_free(&procs), ret);
		ret = execute(procs, env);
		procs_free(&procs);
		if (ret)
			return (ret);
		i += skip_pipeline(get_pipeline_type(line + i));
	}
	return (0);
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
		else if (ret > 0)
			continue ;
	}
	free(line);
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
