/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 08:57:17 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/20 23:20:21 by mirsella         ###   ########.fr       */
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

extern unsigned char	g_exit_code;

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
// else if type == SUBSHELL. args == NULL, path == NULL, line == NULL.
typedef struct s_proc
{
	t_type			type;
	char			*line;
	t_list			*args;
	char			*path;
	struct s_proc	*procs;
	t_next_pipeline	next_pipeline;
	int				fd_in;
	int				fd_out;
	int				pipes[2];
	pid_t			pid;
	int				exit_code;
	struct s_proc	*next;
}				t_proc;

// parsing/parse.c
int				parse(char *line, t_list *env, t_proc **first,
					t_proc *last_proc);
char			*get_next_token(char *line, int *index);

// parsing/pipeline_type.c
int				next_pipeline(char *line);
int				skip_pipeline(t_next_pipeline pipeline_type);
t_next_pipeline	get_pipeline_type(char *line);

// signals.c
void			call_sigaction(void);

// close.c
void			free_shell_data(t_list *env);
void			exit_shell(t_list *env);
void			exit_shell_error(t_list *env, char *msg);

// logging.c
int				print_syntax_error(char *message, char optional);
int				print_error(char *msg, char *optional);
int				print_error_char(char *msg, char optional);

// lstproc.c
int				create_and_push_proc(
					t_proc **first, t_proc **last_proc, t_proc **proc);
t_proc			*new_proc(void);
void			procs_free(t_proc **proc);

// parsing/skipping.c
int				skip_quotes(char *line);
int				skip_parenthesis(char *line);

// parsing/check_unclosed_and_invalid_pipeline.c
int				check_unclosed_and_invalid_pipeline(char *line);

// env.c
int				set_env_var(t_list *env, char *variable, char *value);
int				replace_env_var(t_list *env, char *variable, char *value);
char			*get_env_var(t_list *env, char *variable);
int				set_exit_code_to_env(t_list *env);
int				remove_env_var(t_list *env, char *variable);

// parsing/parse_redirections.c
char			*get_redirect_word_expand(char *line, int *ret, t_list *env);
int				parse_redirections(char *line, t_proc *proc, t_list *env);

// parsing/handle_redirections.c
int				output_redirection(char *line, t_proc *proc, t_list *env);
int				input_redirection(char *line, t_proc *proc, t_list *env);

// parsing/heredoc.c
int				heredoc_redirection(char *line, t_proc *proc, t_list *env);

// parsing/handle_expantion.c
char			*expand_one(char *line, t_list *env, int *index);
char			*expand_vars(char *line, t_list *env);
char			*expand_vars_in_double_quote(
					char *line, int *index, t_list *env);
char			*remove_quotes(char *line);
char			*expand_everything(char *str, t_list *env);

// parsing/expanders.c
char			*expand_var(t_list *env, char *str, int *index);
char			*get_in_quote(char *str, int *index);

// parsing/expand_wildcard.c
int				is_wildcard(char *line);
char			*expand_wildcard(char *line, int *i);
char			*expand_wildcards(char *line);

// parsing/get_dir_content.c
t_list			*get_lst_of_dir(char *path);

// parsing/wildcard_match.c
char			*get_matching_files(char *pattern);

// parsing/parse_command.c
int				parse_command(char *line, t_proc *proc, t_list *env);
int				parse_line_to_proc(char *line, t_proc *proc, t_list *env);

// parsing/set_full_path.c
int				set_full_path(t_list *env, char *cmd, char **full_path);

// execution/execute.c
int				print_procs(t_proc *procs, t_list *env, int layer);
int				execute(t_proc *procs, t_list *env);

// builtin/builtin.c
int				isbuiltin(char *cmd);

// builtin/echo.c
int				builtin_echo(t_proc *proc);

// parsing/stat.c
int				is_file_executable(char *path);
int				is_file_readable(char *path);
int				is_file_writable(char *path);

#endif
