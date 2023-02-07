/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 08:57:17 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/07 23:39:57 by mirsella         ###   ########.fr       */
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

# define PROMPT "minishell> "

// need to add redirections, pipe, etc...
typedef struct s_proc
{
	char			*args;
	char			*path;
	t_list			*env;
	struct s_proc	*next;
}				t_proc;

typedef struct s_data
{
	t_list	*env;
	t_proc	*procs;
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
