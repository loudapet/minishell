/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:53:16 by plouda            #+#    #+#             */
/*   Updated: 2023/07/19 14:08:04 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_built(char **args, char **env, int status)
{
	int	i;

	i = 0;
	args = args;
	env = env;
	if (args[1] && args[2])
	{
		ft_putstr_fd(" too many arguments", 2);
		exit (1);
	}
	if (args[1])
	{
		if (args[1][0] == '-' || args[1][0] == '+')
			i++;
		while (args[1][i])
		{
			if (!ft_isdigit(args[1][i]))
			{
				ft_putstr_fd(" numeric argument required", 2);
				exit (2);
			}
			i++;
		}
		if (args[1][0] == '-')
			exit(156);
		exit(ft_atoi(args[1]));
	}
	exit(status);
}
