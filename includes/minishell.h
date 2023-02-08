/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 08:57:17 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/08 19:03:56 by mirsella         ###   ########.fr       */
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
// don't know between pipes or fd_in/out yet,
// we'll see later when we'll implement pipes
typedef struct s_proc
{
	t_type			type;
	char			*args;
	char			*path;
	struct s_proc	*procs;
	t_next_pipeline	next_pipline;
	int				fd_in;
	int				fd_out;
	int				pipes[2];
	pid_t			pid;
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
	int			original_stderr;
	t_list		*env;
	t_proc		*procs;
}				t_data;

// parse.c
int		parse(char *line, char **envp);

// signals.c
void	call_sigaction(void);

// close.c
void	exit_shell(t_data *data);

// ft_lst.c
t_proc	*new_proc(char *args, t_list *env);
void	add_proc(t_proc *proc, char *args, t_list *env);
void	procs_free(t_proc **proc);

extern int	g_exit_code;

#endif
