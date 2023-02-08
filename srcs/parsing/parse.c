/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgillard <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:01:02 by lgillard          #+#    #+#             */
/*   Updated: 2023/02/09 00:00:34 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse(t_data *data, char *line)
{
	printf("line: '%s'\n", line);
	// take the substring from start to first meta char: |, &&, ||
	while ()
	{
		// check for redirections: <, <<, >, >>. jump past (), "", ''. stop at |, &&, ||, \0. expand var  and quotes which are after redir char. replace redir chars with spaces.
		// if parentheses, call parser recursively with the content of the parentheses. replace parentheses with spaces.
		// else there should be a command here
		//  - expand quotes and vars
		//  - copy the command to the struct
		//  - copy args to the struct
		// should be end of the string
	}
	return (0);
}
/*
   support for : >, >>, <, <<, |, &&, ||, (), ', "
ex: ls -l > file.txt && cat file.txt | grep -i "hello" > file2.txt
ex: << EOF (ls -l > file.txt) && (cat file.txt) | (grep -i "hello") > file2.txt
*/
