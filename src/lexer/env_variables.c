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

int	variable_exists(char *arg, char **env)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '=')
			return (0);
		i++;
	}
	i = 0;
	while (env[i])
	{
		if ((!ft_strncmp((env)[i], arg, ft_strlen(arg))
			&& (env[i][ft_strlen(arg)] == '\0'
			|| env[i][ft_strlen(arg)] == '=')))
			return (1);
		i++;
	}
	return (0);
}

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

int	set_new_pwd(char ***env)
{
	char	*pwd;
	char	**arg;

	if (get_env("PWD", *env))
	{
		arg = malloc(sizeof(char *) * 3);
		pwd = malloc(10000);
		getcwd(pwd, 10000);
		arg[0] = ft_strdup("export");
		arg[1] = ft_strjoin("PWD=", pwd);
		arg[2] = NULL;
		export_built(arg, env);
		free(arg[1]);
		free(arg[0]);
		free(arg);
		free(pwd);
	}
	return (0);
}
