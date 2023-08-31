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

void	show_var(char ***env)
{
	int	i;
	int	j;

	i = 0;
	while ((*env)[i] != NULL)
	{
		j = 0;
		ft_printf("declare -x ");
		while ((*env)[i][j])
		{
			if ((*env)[i][j] == '=' && !((*env)[i][j + 1]))
				break ;
			ft_printf("%c", (*env)[i][j]);
			if ((*env)[i][j] == '=')
				ft_printf("\"");
			j++;
		}
		if ((*env)[i][j] != '=')
			ft_printf("\"");
		ft_printf("\n");
		i++;
	}
}

char	**set_new_env(char *arg, char **env)
{
	char	**temp;
	int		i;

	i = 0;
	while (env[i])
		i++;
	temp = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (env[i] != NULL)
	{
		temp[i] = ft_strdup(env[i]);
		free(env[i]);
		i++;
	}
	temp[i] = ft_strdup(arg);
	temp[i + 1] = NULL;
	free(env);
	return (temp);
}
