/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 17:20:38 by dly               #+#    #+#             */
/*   Updated: 2023/02/27 19:04:33 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(t_proc *proc)
{
	char	buf[PATH_MAX];

	if (!getcwd(buf, PATH_MAX))
		return (print_errorendl("error retrieving current directory",
			strerror(errno)), 1);
	if (ft_putendl_fd(buf, proc->fd_out) == -1)
	{
		print_errorendl("write", strerror(errno));
		return (1);
	}
	return (0);
}
