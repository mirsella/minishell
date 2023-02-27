/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:01:01 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/27 22:13:25 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_pipe1(t_proc *proc);

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
		// g_exit_code = proc->exit_code;
		return ;
	}
	// printf("path : %s  \n pipe[0]: %d && pipe[1]: %d \n fd_in: %d  && fd_out: %d\n\n", proc->path, proc->pipes[0]
	// 		,proc->pipes[1], proc->fd_in, proc->fd_out);
	// printf("forking line %s\n", proc->line);
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

void	wait_loop(t_proc *tmp, t_proc *proc, t_list *env)
{
	int	status;

	close_pipe(tmp);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (tmp)
	{
		if (tmp->type == COMMAND && tmp->path && tmp->pid != -1)
		{
			status = 0;
			waitpid(tmp->pid, &status, 0);
			if (WIFEXITED(status))
				tmp->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				tmp->exit_code = WTERMSIG(status) + 128;
				if (g_exit_code == 131)
					ft_putstr_fd("Quit: (core dumped)\n", STDERR_FILENO);
			}
		}
		g_exit_code = tmp->exit_code;
		if (tmp->next_pipeline == AND || tmp->next_pipeline == OR)
			break ;
		tmp = tmp->next;
	}
	(void)env;
	(void)proc; // moved to process() bc of norm this function is too long
	// if (proc)
	// recursive_and_or(proc, env, 0);
}

int	execute(t_proc *procs, t_list *env)
{
	if (open_pipe(procs))
		return (-1);
	signal(SIGQUIT, SIG_DFL);
	process(procs, env);
	return (0);
}
