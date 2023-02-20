/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirsella <mirsella@protonmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 23:27:37 by mirsella          #+#    #+#             */
/*   Updated: 2023/02/20 23:57:37 by mirsella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_invalid_identifier(char *str)
{
	g_exit_code = 1;
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

int	builtin_export(t_proc *proc, t_list *env)
{
	t_list	*tmp;
	char	*variable;
	char	*value;

	// if (!proc->args)
	// 	return (builtin_env(proc, env));
	tmp = proc->args->next;
	while (tmp)
	{
		if (!is_valid_identifier(tmp->content))
		{
			print_invalid_identifier(tmp->content);
			return (1);
		}
		variable = ft_strndup(tmp->content,
			ft_strchr(tmp->content, '=') - (char *)tmp->content);
		value = ft_strdup(ft_strchr(tmp->content, '=') + 1);
		if (add_env_var(env, variable, value))
			return (free(variable), free(value), -1);
		free(variable);
		free(value);
		tmp = tmp->next;
	}
	return (0);
}
