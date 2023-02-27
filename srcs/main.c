/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:53:10 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/27 12:13:19 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>
#include <unistd.h>

// because the exit codes are between 0 and 255. it overflow if > 255
unsigned char	g_exit_code;

int	init_shell(t_list **env, char **envp)
{
	char	**env_tab;
	char	*tmp;
	t_list	*lst;

	g_exit_code = 0;
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
	tmp = ft_strdup("");
	if (!tmp)
		return (perror("malloc"), -1);
	lst = ft_lstnew(tmp);
	if (!lst)
		return (perror("malloc"), -1);
	ft_lstadd_front(env, lst);
	return (0);
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
		call_sigaction_readline();
		line = readline(PROMPT);
		if (!line)
			break ;
		if (!*(line + ft_skip_spaces(line)))
			continue ;
		if (!ft_isspace(*line))
			add_history(line);
		ret = handle_line(line, env);
		if (ret < 0)
			break ;
		if (ret > 1)
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
