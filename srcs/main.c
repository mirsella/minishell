/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:53:10 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/12 16:59:13 by mirsella         ###   ########.fr       */
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

// dev function
void	print_procs(t_proc *procs, int layer)
{
	t_proc	*tmp;
	char	*next_pipeline;

	tmp = procs;
	while (tmp)
	{
		if (tmp->next_pipeline == PIPE)
			next_pipeline = "PIPE";
		else if (tmp->next_pipeline == OR)
			next_pipeline = "OR";
		else if (tmp->next_pipeline == AND)
			next_pipeline = "AND";
		else
			next_pipeline = "INVALID";
		if (tmp->type == SUBSHELL)
		{
			printf("%*cSUBSHELL: next_pipeline type: %s\n", layer, ' ',
				next_pipeline);
			print_procs(tmp->procs, layer + 4);
		}
		else
			printf("%*cCOMMAND: %s, next_pipeline type: %s\n", layer, ' ',
				tmp->path, next_pipeline);
		tmp = tmp->next;
	}
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
		ret = parse(data, line);
		if (ret < 0)
			break ;
		else if (ret > 0)
			continue ;
		print_procs(data->procs, 0);
		// execute(data);
	}
	free(line);
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
