/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 00:13:48 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/10 00:14:13 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_syntax_error(char *message, char optional)
{
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	if (optional)
		ft_putchar_fd(optional, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}

void	print_error(char *msg, char optional)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	if (optional)
		ft_putchar_fd(optional, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
