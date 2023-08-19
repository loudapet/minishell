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

int	cd_home(char ***env)
{
	char	*pwd;
	char	**arg;

	arg = malloc(sizeof(char *) * 3);
	pwd = malloc(10000);
	env = env;
	getcwd(pwd, 10000);
	arg[0] = ft_strdup("export");
	arg[1] = ft_strjoin("OLDPWD=", pwd);
	arg[2] = NULL;
	chdir(get_env("HOME", *env));
	if (get_env("OLDPWD", *env))
		export_built(arg, env);
	free(arg[1]);
	free(arg[0]);
	free(arg);
	free(pwd);
	return (0);
}

int	set_old_pwd(char *pwd, char **env)
{
	char	**arg;

	if (get_env("OLDPWD", env))
	{
		arg = malloc(sizeof(char *) * 3);
		arg[0] = ft_strdup("export");
		arg[1] = ft_strjoin("OLDPWD=", pwd);
		arg[2] = NULL;
		export_built(arg, &env);
		free(arg[1]);
		free(arg[0]);
		free(arg);
	}
	return (0);
}

int	cd_squigly(char *argv, char ***env)
{
	char	*arg;

	arg = ft_strjoin(get_env("HOME", *env), argv + 1);
	if (chdir(arg) == 0)
	{
		set_new_pwd(*env);
		free(arg);
		return (1);
	}
	free(arg);
	return (0);
}

void	set_pwd_variables(char *pwd, char ***env)
{
	set_new_pwd(*env);
	set_old_pwd(pwd, *env);
}

int	cd_built(char **argv, char ***env)
{
	char	*pwd;

	pwd = malloc(10000);
	getcwd(pwd, 10000);
	if (!argv[1])
	{
		if (!get_env("HOME", *env))
			ft_putstr_fd("HOME not set\n", 2);
		if (!get_env("HOME", *env))
			return (0);
		cd_home(env);
		set_new_pwd(*env);
		return (0);
	}
	if (argv[2])
	{
		ft_putstr_fd(" too many arguments\n", 2);
		return (1);
	}
	if (argv[1][0] == '~')
	{
		if (cd_squigly(argv[1], env))
			set_old_pwd(pwd, *env);
		free(pwd);
		return (0);
	}
	if (chdir(argv[1]) == 0)
		set_pwd_variables(pwd, env);
	else
	{
		ft_putstr_fd(" No such file or directory\n", 2);
		return (1);
	}
	free(pwd);
	return (0);
}
