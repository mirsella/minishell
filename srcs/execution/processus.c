/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:17:06 by dly               #+#    #+#             */
/*   Updated: 2023/02/27 21:11:37 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>
#include <stdio.h>

void	close_pipe1(t_proc *proc);
void	assign_pipe(t_proc *proc);

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

static void	child(t_proc *tmp, t_proc *proc, t_list *env)
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

static void	wait_loop(t_proc *tmp, t_proc *proc, t_list *env)
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

int	cmd_not_found(t_proc *proc)
{
	if (!proc->path && proc->type == COMMAND)
	{
		if (proc->exit_code == 0)
			proc->exit_code = 127;
		else if (proc->exit_code == -1)
			proc->exit_code = 0;
		return (1);
	}
	return (0);
}

int	process(t_proc *proc, t_list *env)
{
	t_proc	*tmp;

	tmp = proc;
	// if (proc)
	// 	printf("in process type %d line %s\n", proc->type, proc->line);
	// else
	// 	printf("proc is NULL\n");
	while (proc)
	{
		parse_line_to_proc(proc->line, proc, env);
		// printf("parselinetoproc filled proc type %d line %s and path %s\n", proc->type, proc->line, proc->path);
		// if (proc->type == SUBSHELL)
		// 	printf("  procs: %p\n", proc->procs);
		// printf("path : %s  line: %s \n pipe[0]: %d && pipe[1]: %d \n fd_in: %d  && fd_out: %d\n\n"
		// 	,proc->path, proc->line, proc->pipes[0] ,proc->pipes[1], proc->fd_in, proc->fd_out);
		if (!cmd_not_found(proc) && (proc->path || proc->type == SUBSHELL))
		{
			assign_pipe(proc);
			if (proc->type == COMMAND && proc->fd_in != -1)
			{
				// printf("calling child %s\n", proc->line);
				child(tmp, proc, env);
			}
			if (proc->type == SUBSHELL)
			{
				// printf("calling process recursively bc of SUBSHELL %s and procs %p\n", proc->line, proc->procs);
				process(proc->procs, env);
			}
		}
		if (proc->next_pipeline == AND || proc->next_pipeline == OR)
			break ;
		proc = proc->next;
	}
	wait_loop(tmp, proc, env);
	if (proc)
		recursive_and_or(proc, env, 0);
	return (0);
}
