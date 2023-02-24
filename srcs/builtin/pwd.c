/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dly <dly@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 17:20:38 by dly               #+#    #+#             */
/*   Updated: 2023/02/24 16:13:19 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(t_proc *proc)
{
	char	buf[PATH_MAX];

	if (!getcwd(buf, PATH_MAX))
		return (-1);
	if (ft_putendl_fd(buf, proc->fd_out) == -1)
	{
		print_errorendl("write", strerror(errno));
		return (1);
	}
	return (0);
}
