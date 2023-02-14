/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:53:10 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/14 21:38:31 by mirsella         ###   ########.fr       */
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
	data->original_stdin = dup(STDIN_FILENO);
	data->original_stdout = dup(STDOUT_FILENO);
	if (data->original_stdin == -1 || data->original_stdout == -1)
		return (perror("dup"), -1);
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

// return > 0 mean parsing error, show new prompt
// return < 0 mean fatal error, exit
int	handle_line(t_data *data, char *line, t_proc *last_proc)
{
	char			*cmd;
	t_proc			*proc;
	int				ret;

	if (check_unclosed(line))
		return (1);
	while (*line)
	{
		cmd = line;
		if (init_cmd_and_proc(&proc, &cmd, data, last_proc) < 0)
			return (-1);
		proc->next_pipeline = get_pipeline_type(line + next_pipeline(line));
		ret = parse_redirections(data, cmd, proc);
		if (ret)
			return (free(cmd), ret);
		ret = parse_command_or_subshell(data, cmd, proc);
		if (ret)
			return (free(cmd), ret);
		free(cmd);
		line += next_pipeline(line) + skip_pipeline(proc->next_pipeline);
		if (!is_nextpipeline_possible(proc->next_pipeline, line))
			return (1);
		last_proc = proc;
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
		if (execute(data) < 0)
			break ;
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
