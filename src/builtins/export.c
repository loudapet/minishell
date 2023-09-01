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

// static void	show_var(char ***env)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	while ((*env)[i] != NULL)
// 	{
// 		j = 0;
// 		ft_printf("declare -x ");
// 		while ((*env)[i][j])
// 		{
// 			if ((*env)[i][j] == '=' && !((*env)[i][j + 1]))
// 				break ;
// 			ft_printf("%c", (*env)[i][j]);
// 			if ((*env)[i][j] == '=')
// 				ft_printf("\"");
// 			j++;
// 		}
// 		if ((*env)[i][j] != '=')
// 			ft_printf("\"");
// 		ft_printf("\n");
// 		i++;
// 	}
// }

// static char	**set_new_env(char *arg, char **env)
// {
// 	char	**temp;
// 	int		i;

// 	i = 0;
// 	while (env[i])
// 		i++;
// 	temp = malloc(sizeof(char *) * (i + 2));
// 	i = 0;
// 	while (env[i] != NULL)
// 	{
// 		temp[i] = ft_strdup(env[i]);
// 		free(env[i]);
// 		i++;
// 	}
// 	temp[i] = ft_strdup(arg);
// 	temp[i + 1] = NULL;
// 	free(env);
// 	return (temp);
// }

static int	check_arg(char *arg)
{
	int	i;

	i = 0;
	if (!arg[0] || arg[0] == '=')
		return (0);
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalpha(arg[i]))
			return (0);
		i++;
	}
	if (arg[i] != '=')
		return (2);
	return (1);
}

char	*get_env_name(char *arg)
{
	int		i;
	char	*name;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	name = malloc(sizeof(char) * i + 1);
	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		name[i] = arg[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

void	export_extend(char **argv, char ***env, int *status)
{
	char	*name;
	int		i;

	i = 1;
	while (argv[i])
	{
		if (check_arg(argv[i]) != 0)
		{
			if (check_arg(argv[i]) == 2)
				argv[i] = ft_strjoin_custom(argv[i], "=");
			name = get_env_name(argv[i]);
			*env = unset_single(name, *env);
			*env = set_new_env(argv[i], *env);
			free(name);
		}
		else
		{
			*status = 1;
			ft_putstr_fd(" not a valid identifier\n", 2);
		}
		i++;
	}
}

int	export_built(char **argv, char ***env)
{
	int	status;

	status = 0;
	if (!argv[1])
	{
		show_var(env);
		return (status);
	}
	export_extend(argv, env, &status);
	return (status);
}
