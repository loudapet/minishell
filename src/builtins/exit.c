/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:53:16 by plouda            #+#    #+#             */
/*   Updated: 2023/08/25 10:27:03 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_norm(char **args, int i)
{
	if (args[1])
	{
		if (args[1][0] == '-' || args[1][0] == '+')
			i++;
		while (args[1][i])
		{
			if (!ft_isdigit(args[1][i]))
			{
				ft_putstr_fd(" numeric argument required\n", 2);
				exit (2);
			}
			i++;
		}
		if (args[1][0] == '-')
			exit(156);
		exit(ft_atoi(args[1]));
	}
}

void	exit_built(char **args, char **env, int status)
{
	int	i;

	i = 0;
	//args = args;
	//env = env;
	(void)args;
	(void)env;
	if (args[1] && args[2])
	{
		ft_putstr_fd(" too many arguments\n", 2);
		return ;
	}
	exit_norm(args, i);
	exit(status);
}
