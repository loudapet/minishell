/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:53:16 by plouda            #+#    #+#             */
/*   Updated: 2023/07/17 11:10:31 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_var_name(char *str, int start)
{
	int	i;
	int pos;
	char *name;

	i = 0;
	pos = start + 1;
	while (str[pos] != '\0' && str[pos] != ' ' && str[pos] != '$')
	{
		//ft_printf("COUNTING...\n");
		i++;
		pos++;
	}
	name = malloc(sizeof(char) * i + 1);
	if (!name)
		return (NULL);
	i = 0;
	start++; // no $ needed anymore
	while (start < pos)
	{
		//ft_printf("EXPANDING...\n");
		name[i] = str[start];
		start++;
		i++;
	}
	name[i] = '\0';
	return (name);
}

void	display_env(char **env)
{
	int	i;
	int	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j])
		{
			write(1, &env[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}

char	*expand_env(char *str, char **env)
{
	int		i;
	char	*var_name;
	char	*var;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			var_name = get_env_var_name(str, i);
			ft_printf("Variable name: %s\n", var_name);
			var = get_env(var_name, env);
			ft_printf("Expanded variable: %s\n", var);
		}
		i++;
	}
	return (str);
}

char	*sanitize_double_quotes(const char *str)
{
	char	*str_san;
	int		i;
	int		j;
	int		quote_counter;

	i = 0;
	quote_counter = 0;
	while (str[i])
	{
		if (str[i] == '"')
			quote_counter++;
		i++;
	}
	i = 0;
	str_san = malloc(sizeof(char *) * (ft_strlen(str) - quote_counter + 1)); // protecc
	j = 0;
	while (str[i])
	{
		while (str[i] == '"')
			i++;
		str_san[j] = str[i];
		i++;
		j++;
	}
	str_san[j] = '\0';
	free((char *)str);
	return (str_san);
}

char	*sanitize_single_quotes(const char *str)
{
	char	*str_san;
	int		i;
	int		j;
	int		quote_counter;

	i = 0;
	quote_counter = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			quote_counter++;
		i++;
	}
	i = 0;
	str_san = malloc(sizeof(char *) * (ft_strlen(str) - quote_counter + 1)); // protecc
	j = 0;
	while (str[i])
	{
		while (str[i] == '\'')
			i++;
		str_san[j] = str[i];
		i++;
		j++;
	}
	str_san[j] = '\0';
	free((char *)str);
	return (str_san);
}

char	**sanitizer(int ac, char **av, char **env)
{
	int	i;
	int	j;
	char	**argv;
	int	flag;

	i = 0;
	flag = 0;
	argv = malloc(sizeof(char **) * ac);
	while (av[i])
	{
		j = 0;
		while (av[i][j] && !flag)
		{
			if (av[i][j] == '\'')
			{
				argv[i] = sanitize_single_quotes(av[i]);
				flag = 1;
			}
			else if (av[i][j] == '"')
			{
				argv[i] = sanitize_double_quotes(av[i]);
				flag = 2;
			}
			j++;
		}
		if (!flag)
			argv[i] = ft_strdup(av[i]);
		if (!flag || flag == 2)
			expand_env(argv[i], env);
		flag = 0;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

void	lexer(const char *line, char **env)
{
	char	**av;
	int		ac;
	int		i;
	int		j;
	char	**argv;

	av = minisplit(line, ' ');
	ac = 0;
	while (av[ac])
	{
		i = 0;
		while (av[ac][i])
		{
			write(1, &av[ac][i], 1);
			i++;
		}
		write(1, "\n", 1);
		ac++;
	}
	ac++;

	ft_printf("Sanitizing...\n");
	// free av after sanitizing!
	display_env(env);
	argv = sanitizer(ac, av, env);
	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			write(1, &argv[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}