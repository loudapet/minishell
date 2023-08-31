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

int	count_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

char	**unset_single(char *arg, char **env)
{
	char	**temp;
	int		i;
	int		l;

	l = 0;
	i = 0;
	if (!variable_exists(arg, env))
		return (env);
	temp = malloc(sizeof(char *) * count_env(env));
	while (env[i])
	{
		if (!(!ft_strncmp((env)[i], arg, ft_strlen(arg))
			&& (env[i][ft_strlen(arg)] == '\0'
			|| env[i][ft_strlen(arg)] == '=')))
		{
			temp[l] = ft_strdup((env)[i]);
			l++;
		}
		free((env)[i]);
		i++;
	}
	temp[l] = NULL;
	free(env);
	return (temp);
}

char	**unset_built(char **arg, char **env)
{
	int	i;

	i = 1;
	while (arg[i])
	{
		env = unset_single(arg[i], env);
		i++;
	}
	return (env);
}
