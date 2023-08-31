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

int	env_built(char **env)
{
	int	i;
	int	l;

	i = 0;
	while (env[i])
	{
		l = 0;
		while (env[i][l])
		{
			if (env[i][l] == '=' && env[i][l + 1] != '\0')
			{
				ft_printf("%s\n", env[i]);
				break ;
			}
			l++;
		}
		i++;
	}
	return (0);
}
