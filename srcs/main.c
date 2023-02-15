/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:53:10 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/15 11:56:15 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>
#include <unistd.h>

int	g_exit_code;

int	init_shell(t_data *data, char **envp)
{
	char	**env;

	g_exit_code = 0;
	call_sigaction();
	env = ft_tabdup(envp);
	if (!env)
		return (perror("malloc"), -1);
	data->env = ft_lstnew_strs(ft_tablen(env), env);
	free(env);
	if (!data->env)
		return (perror("malloc"), -1);
	return (0);
}

char	*get_pipelines(char *line, int *index)
{
	int				i;
	char			*cmd;
	t_next_pipeline	next_pipeline_type;

	i = 0;
	while (line[i])
	{
		i += next_pipeline(line + i);
		next_pipeline_type = get_pipeline_type(line + i);
		if (next_pipeline_type == OR || next_pipeline_type == AND)
			break ;
		i += skip_pipeline(next_pipeline_type);

	}
	cmd = ft_substr(line, 0, i);
	if (!cmd)
		return (perror("malloc"), NULL);
	*index += i;
	return (cmd);
}

int	handle_line(t_data *data, char *line, t_proc *last_proc)
{
	char			*cmd;
	int				i;
	int				ret;

	if (check_unclosed(line)) // shoud also check for syntax error like & or ||| or &&& or |\0
		return (1);
	i = 0;
	while (line[i])
	{
		cmd = get_pipelines(line + i, &i);
		if (!cmd)
			return (-1);
		ret = parse(data, cmd, last_proc);
		free(cmd);
		if (ret)
			return (ret);
		ret = execute(data);
		if (ret)
			return (ret);
		procs_free(&data->procs);
		i += skip_pipeline(get_pipeline_type(line + i));
	}
	return (0);
}

int	prompt_loop(t_data *data)
{
	char	*line;
	int		ret;

	line = NULL;
	while (1)
	{
		free(line);
		procs_free(&data->procs);
		line = readline(PROMPT);
		if (!line)
			break ;
		if (!*(line + ft_skip_spaces(line)))
			continue ;
		add_history_filter(line);
		ret = handle_line(data, line, NULL);
		if (ret < 0)
			break ;
		else if (ret > 0)
			continue ;
	}
	free(line);
	procs_free(&data->procs);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	bzero(&data, sizeof(t_data));
	if (init_shell(&data, envp))
		exit_shell(&data);
	prompt_loop(&data);
	exit_shell(&data);
}
