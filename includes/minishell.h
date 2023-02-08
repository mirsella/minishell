/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 08:57:17 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/08 23:50:04 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h" // ft_*
# include "stdlib.h" // free, exit
# include "stdio.h" // printf, perror
# include "readline/readline.h" // readline
# include "readline/history.h" // add_history
# include "signal.h" // sigaction
# include "unistd.h" // fork, execve, chdir, getcwd, access, close, dup2, pipe
# include "errno.h" // errno
# include <sys/types.h> // pid_t

# define PROMPT "minishell> "

typedef enum e_next_pipeline
{
	PIPE,
	AND,
	OR,
}				t_next_pipeline;

typedef enum e_type
{
	SUBSHELL,
	COMMAND,
}				t_type;

// represent a command or a subshell
//
// if type == COMMAND. procs == NULL.
// else if type == SUBSHELL. args == NULL, path == NULL
//
// don't know between pipes[2] or fd_in/out yet,
// we'll see later when we'll implement pipes
// rappel: pipes[0] == read, pipes[1] == write
typedef struct s_proc
{
	t_type			type;
	char			*args;
	char			*path;
	struct s_proc	*procs;
	t_next_pipeline	next_pipeline;
	int				fd_in;
	int				fd_out;
	int				pipes[2];
	pid_t			pid;
	int				status;
	struct s_proc	*next;
}				t_proc;

// t_proc *procs: represent the commands or subshell to exec.
// ex : (ls | grep a) && (ls | grep b) would be :
// 2 elements in the list of type SUBSHELL:
// each element have 2 t_proc of type COMMAND, one for ls and one for grep
typedef struct s_data
{
	int			original_stdin;
	int			original_stdout;
	t_list		*env;
	t_proc		*procs;
}				t_data;

// parse.c
int		parse(t_data *data, char *line);

// signals.c
void	call_sigaction(void);

// close.c
void	free_shell_data(t_data *data);
void	exit_shell(t_data *data);
void	exit_shell_error(t_data *data, char *msg);

// ft_lst.c
t_proc	*new_proc_command(char *args, char *path,
			t_next_pipeline next_pipeline, int pipes[2]);
t_proc	*new_proc_subshell(t_proc *procs,
			t_next_pipeline next_pipeline, int pipes[2]);
void	push_back_proc(t_proc *procs, t_proc *proc);
void	procs_free(t_proc **proc);

extern int	g_exit_code;

#endif
