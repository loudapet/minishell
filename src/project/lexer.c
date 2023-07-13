/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:53:16 by plouda            #+#    #+#             */
/*   Updated: 2023/07/13 14:06:58 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lexer(const char *line)
{
	char	**av;
	int		ac;
	int		i;

	av = minisplit(line, ' ');
	ac = 0;
	while (av[ac])
	{
		i = 0;
		while (av[ac][i])
		{
			write(1, &av[ac][i], 1);
			i++;
		}
		write(1, "\n", 1);
		ac++;
	}
}