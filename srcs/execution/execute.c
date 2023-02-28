/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:01:01 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/28 14:18:50 by lgillard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_and_exit_child(t_proc *proc, t_list *env, int exit_code)
{
	ft_lstclear(&env, free);
	procs_free(&proc);
	exit(exit_code);
}

int	launch_command(t_proc *proc, t_list *env)
{
	char	**args;
	char	**envp;

	args = ft_lst_to_tab(proc->args);
	if (!args)
		return (perror("malloc"), -1);
	envp = ft_lst_to_tab(env);
	if (!envp)
		return (free(args), perror("malloc"), -1);
	if (execve(proc->path, args, envp) == -1)
	{
		print_errorendl(proc->path, strerror(errno));
		ft_free_tab(args);
		ft_free_tab(envp);
		if (errno == ENOENT)
			return (127);
		else if (errno == EACCES)
			return (126);
		else
			return (1);
	}
	ft_free_tab(args);
	ft_free_tab(envp);
	return (1);
}

void	child(t_proc *tmp, t_proc *proc, t_list *env)
{
	if (isbuiltin(proc->path) && (!tmp->next || tmp->next_pipeline == AND
			|| tmp->next_pipeline == OR))
	{
		proc->exit_code = exec_builtin(proc, env);
		return ;
	}
	proc->pid = fork();
	if (!proc->pid)
	{
		if (double_dup2(proc->fd_in, proc->fd_out) == -1)
			free_and_exit_child(proc, env, 1);
		if (isbuiltin(proc->path))
		{
			proc->exit_code = exec_builtin(proc, env);
			free_and_exit_child(proc, env, proc->exit_code);
		}
		close_pipe(tmp);
		if (tmp && tmp->prev)
		{
			while (tmp->prev)
				tmp = tmp->prev;
		}
		close_pipe1(tmp);
		free_and_exit_child(proc, env, launch_command(proc, env));
	}
}

void	wait_loop(t_proc *proc)
{
	int	status;

	close_pipe(proc);
	signal(SIGINT, &sigint_process);
	signal(SIGQUIT, SIG_IGN);
	while (proc)
	{
		if (proc->type == COMMAND && proc->path && proc->pid != -1)
		{
			status = 0;
			waitpid(proc->pid, &status, 0);
			if (WIFEXITED(status))
				proc->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				proc->exit_code = WTERMSIG(status) + 128;
				if (proc->exit_code == 131)
					ft_putstr_fd("Quit: (core dumped)\n", STDERR_FILENO);
			}
		}
		g_exit_code = proc->exit_code;
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break ;
		proc = proc->next;
	}
}

int	execute(t_proc *procs, t_list *env)
{
	int	ret;

	if (open_pipe(procs))
		return (-1);
	signal(SIGQUIT, SIG_DFL);
	ret = process(procs, env);
	return (ret);
}
