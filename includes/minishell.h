/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 08:57:17 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/06 15:58:24 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "stdlib.h"
# include "stdio.h"
# include "readline/readline.h"
# include "readline/history.h"

# define PROMPT "minishell> "

int	parse(char *line, char **envp);

#endif
