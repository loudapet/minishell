/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slicer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehasalu <ehasalu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 14:43:42 by plouda            #+#    #+#             */
/*   Updated: 2023/09/01 17:49:52 by ehasalu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_changer(char **av, int i, int j, int *quote)
{
	if (av[i][j] == '\'' && *quote == 0)
		*quote = 1;
	else if (av[i][j] == '\'' && *quote == 1)
		*quote = 0;
	if (av[i][j] == '"' && *quote == 0)
		*quote = 2;
	else if (av[i][j] == '"' && *quote == 2)
		*quote = 0;
}

void	state_changer(char **av, int i, int j, int *state)
{
	if ((av[i][j] == '>' || av[i][j] == '<'
		|| av[i][j] == '|') && *state == 0)
		*state = 1;
	else if (*state == 0)
		*state = 2;
}
