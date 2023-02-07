/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 08:57:17 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/07 17:08:49 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h" // ft_*
# include "stdlib.h" // free, exit
# include "stdio.h" // printf
# include "readline/readline.h" // readline()
# include "readline/history.h" // add_history
# include "signal.h" // sigaction

# define PROMPT "minishell> "

// need to add redirections, pipe, etc...
typedef struct s_proc
{
	char			*path;
	char			*args;
	struct s_proc	*next;
	struct s_proc	*prev;
}				t_proc;

// parse.c
int		parse(char *line, char **envp);

// signals.c
void	call_sigaction(void);

extern int	g_exit_code;

#endif
