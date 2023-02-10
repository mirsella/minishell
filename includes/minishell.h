/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 08:57:17 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/10 16:37:04 by mirsella         ###   ########.fr       */
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
# include "unistd.h" // fork, execve, chdir, getcwd, access, dup2, pipe
# include "fcntl.h" // open flags
# include "errno.h" // errno
# include <sys/types.h> // pid_t

# define PROMPT "minishell$ "

typedef enum e_next_pipeline
{
	INVALID,
	PIPE,
	AND,
	OR,
}				t_next_pipeline;

typedef enum e_type
{
	COMMAND,
	SUBSHELL,
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
	t_list			*args;
	char			*path;
	struct s_proc	*procs;
	t_next_pipeline	next_pipeline;
	int				fd_in;
	int				fd_out;
	int				pipes[2];
	pid_t			pid;
	int				exit_status;
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
int				parse(t_data *data, char *line, t_proc *proc);
int				ismeta(char c);

// pipeline_type.c
int				next_pipeline(char *line);
int				skip_pipeline(t_next_pipeline pipeline_type);
t_next_pipeline	get_pipeline_type(char *line);

// redirections.c
int				handle_redirections(t_data *data, char *line, t_proc *proc);

// signals.c
void			call_sigaction(void);

// close.c
void			free_shell_data(t_data *data);
void			exit_shell(t_data *data);
void			exit_shell_error(t_data *data, char *msg);

// logging.c
void			print_syntax_error(char *message, char optional);
void			print_error(char *msg, char *optional);
void			print_error_char(char *msg, char optional);

// ft_lst.c
t_proc			*create_and_push_proc(t_data *data, t_proc *last_proc);
t_proc			*new_proc(void);
void			push_back_proc(t_proc *procs, t_proc *proc);
void			procs_free(t_proc **proc);

// skipping.c
int				skip_quotes(char *line);
int				skip_parenthesis(char *line);

// prompt_loop_utils.c
void			add_history_filter(char *line);
int				check_unclosed(char *line);

extern int	g_exit_code;

#endif
