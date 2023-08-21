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

int	has_path(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

void	exe_with_path(char **path, char **argv, char ***env)
{
	int		i;
	char	*slash;
	char	*command_path;

	i = 0;
	while (path[i])
	{
		slash = ft_strjoin(path[i], "/");
		command_path = ft_strjoin(slash, argv[0]);
		if (access(command_path, F_OK) == 0)
			execve(command_path, argv, *env);
		free(command_path);
		free(slash);
		i++;
	}
}

int	exec_command(char **argv, char ***env)
{
	char	**path;
	int		i;

	i = 0;
	if (has_path(argv[0]))
	{
		if (access(argv[0], F_OK) == 0)
			execve(argv[0], argv, *env);
		else
			return (0);
	}
	if (get_env("PATH", *env))
		path = ft_split(get_env("PATH", *env), ':');
	else
		return (0);
	exe_with_path(path, argv, env);
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
	return (0);
}

void	builtins(char	**argv, char ***env, int *status)
{
	if (!ft_strncmp(argv[0], "echo", 5))
		*status = echo_built(argv);
	else if (!ft_strncmp(argv[0], "cd", 3))
		*status = cd_built(argv, env);
	else if (!ft_strncmp(argv[0], "export", 7))
		*status = export_built(argv, env);
	else if (!ft_strncmp(argv[0], "unset", 6))
		*env = unset_built(argv, *env);
	else if (!ft_strncmp(argv[0], "env", 4))
		env_built(*env);
	else if (!ft_strncmp(argv[0], "pwd", 4))
		pwd_built(*env);
	else if (!ft_strncmp(argv[0], "exit", 5))
		exit_built(argv, *env, *status);
	else
		exec_command(argv, env);
}
