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

int	echo_built(char	**args)
{
	int	i;

	if (args[1] && !ft_strncmp(args[1],
			"-n", ft_strlen(args[1])))
		i = 2;
	else
		i = 1;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!args[1])
		ft_putchar_fd('\n', 1);
	else if (ft_strncmp(args[1], "-n", ft_strlen(args[1])))
		ft_putchar_fd('\n', 1);
	return (0);
}
