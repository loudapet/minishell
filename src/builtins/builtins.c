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

int	exec_command(char **argv, char ***env)
{
	char	**path;
	char	*slash;
	char	*command_path;

	if (get_env("PATH", *env))
		path = ft_split(get_env("PATH", *env), ':');
	else
		return (0);
	int i = 0;
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
