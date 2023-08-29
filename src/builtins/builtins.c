/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:53:16 by plouda            #+#    #+#             */
/*   Updated: 2023/08/25 13:59:49 by plouda           ###   ########.fr       */
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
	ft_putstr_fd(argv[0], 2);
	ft_putstr_fd(": command not found\n", 2);

}

int	exec_command(char **argv, char ***env)
{
	char	**path;
	int		i;

	i = 0;
	if (has_path(argv[0]))
	{
		execve(argv[0], argv, *env);
		if (errno == 2)
		{
			ft_printf("%s\n", strerror(errno));
			return (127);
		}
		ft_printf("%s\n", strerror(errno));
		return (126);
	}
	if (get_env("PATH", *env))
		path = ft_split(get_env("PATH", *env), ':');
	else
		return (127);
	exe_with_path(path, argv, env);
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
	return (127);
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
		*status =  exec_command(argv, env);
}
