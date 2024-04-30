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
