/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 08:57:17 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/14 14:13:05 by mirsella         ###   ########.fr       */
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
# include "sys/stat.h" // stat
# include "errno.h" // errno
# include "sys/types.h" // pid_t
# include "dirent.h" // opendir, readdir, closedir

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
	t_list		*dir_content;
	t_proc		*procs;
}				t_data;

// parsing/parse.c
int				parse(t_data *data, char *line, t_proc *last_proc);

// parsing/pipeline_type.c
int				is_nextpipeline_possible(
					t_next_pipeline next_pipeline, char *line);
int				next_pipeline(char *line);
int				skip_pipeline(t_next_pipeline pipeline_type);
t_next_pipeline	get_pipeline_type(char *line);

// signals.c
void			call_sigaction(void);

// close.c
void			free_shell_data(t_data *data);
void			exit_shell(t_data *data);
void			exit_shell_error(t_data *data, char *msg);

// logging.c
int				print_syntax_error(char *message, char optional);
int				print_error(char *msg, char *optional);
int				print_error_char(char *msg, char optional);

// proc/ft_lst.c
t_proc			*create_and_push_proc(t_data *data, t_proc *last_proc);
t_proc			*new_proc(void);
t_proc			*get_last_proc(t_proc *procs);
void			push_back_proc(t_proc *procs, t_proc *proc);
void			procs_free(t_proc **proc);

// parsing/skipping.c
int				skip_quotes(char *line);
int				skip_parenthesis(char *line);

// parsing/prompt_loop_utils.c
void			add_history_filter(char *line);
int				check_unclosed(char *line);

// env.c
char			*get_env_value(t_list *env, char *variable);

// parsing/parse_redirections.c
char			*get_word_expand(t_data *data, char *line, int *ret);
int				parse_redirections(t_data *data, char *line, t_proc *proc);

// parsing/handle_redirections.c
int				output_redirection(t_data *data, char *line, t_proc *proc);

// parsing/handle_expantion.c
char			*expand_vars(t_list *env, char *line);
char			*expand_everything(t_list *env, char *str);

// parsing/expanders.c
char			*expand_var(t_list *env, char *str, int *index);
char			*expand_single_quote(char *str, int *index);
char			*expand_double_quote(t_list *env, char *str, int *index);

// parsing/expand_wildcard.c
char			*expand_wildcards(char *line, t_list *env);

// parsing/get_dir_content.c
t_list			*get_lst_of_dir(char *path);

// parsing/wildcard_match.c
char			*get_matching_files(char *pattern);

// parsing/parse_command.c
char			*get_next_token(char *line, int *index);
int				parse_command(t_data *data, char *line, t_proc *proc);

// parsing/set_full_path.c
int				set_full_path(t_list *env, char **cmd);

// execution/execute.c
int				execute(t_data *data);

// builtin/builtin.c
int				isbuiltin(char *cmd);

extern int	g_exit_code;

#endif
