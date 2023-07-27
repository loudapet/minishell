/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 14:31:22 by plouda            #+#    #+#             */
/*   Updated: 2023/07/25 14:32:09 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_env(char **env)
{
	int		i;
	char	**env_arr;

	i = 0;
	while (env[i])
		i++;
	env_arr = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		env_arr[i] = ft_strdup(env[i]);
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

char	*get_env(char *variable, char **env_arr)
{
	int		i;
	size_t	l;
	char	*cpy;

	i = 0;
	while (env_arr[i])
	{
		l = 0;
		while (env_arr[i][l] != '=')
			l++;
		if (!ft_strncmp(variable, env_arr[i], l) && l == ft_strlen(variable))
			break ;
		i++;
	}
	if (!env_arr[i])
		return (NULL);
	cpy = env_arr[i];
	while (*cpy != '=')
		cpy++;
	cpy++;
	if (*cpy)
		return (cpy);
	else
		return (NULL);
}

void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}
