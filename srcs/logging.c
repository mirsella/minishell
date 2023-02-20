/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 00:13:48 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/20 23:37:32 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	print_syntax_error(char *message, char optional)
{
	ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
	if (message)
		ft_putstr_fd(message, STDERR_FILENO);
	if (optional)
	{
		ft_putstr_fd("`", STDERR_FILENO);
		ft_putchar_fd(optional, STDERR_FILENO);
		ft_putstr_fd("'", STDERR_FILENO);
	}
	ft_putchar_fd('\n', STDERR_FILENO);
	return (1);
}

int	print_error(char *msg, char *optional)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	if (optional)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(optional, STDERR_FILENO);
	}
	ft_putstr_fd("\n", STDERR_FILENO);
	return (1);
}
