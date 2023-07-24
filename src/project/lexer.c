/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:53:16 by plouda            #+#    #+#             */
/*   Updated: 2023/07/24 12:27:14 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_var_name(char *str, int start)
{
	int		i;
	int		pos;
	char	*name;

	i = 0;
	pos = start + 1;
	while (str[pos] != '\0' && str[pos] != ' ' && str[pos] != '$'
		&& str[pos] != '"' && str[pos] != '\'') // upgrade to any metacharacter
	{
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
		name[i] = str[start];
		start++;
		i++;
	}
	name[i] = '\0';
	return (name);
}

char	*expand_env(char *str, char **env)
{
	int		i;
	char	*var_name;
	char	*var;
	int		quote;
	int		single_quote;

	i = 0;
	quote = 0;
	single_quote = 0;
	while (str[i])
	{
		quote_counter(&str[i], &quote, &single_quote);
		if (str[i] == '$' && !(single_quote % 2))
		{
			var_name = get_env_var_name(str, i);
			var = get_env(var_name, env);
			str = expand_and_join(str, var_name, var, i);
			i = -1;
			quote = 0;
			single_quote = 0;
			free(var_name);
		}
		i++;
	}
	return (str);
}

void	free_args(t_args args)
{
	int	i;

	i = 0;
	while (i < args.ac)
	{
		free(args.av[i]);
		i++;
	}
	free(args.av);
}

// norm compliant, just delete helper output loops
t_args	lexer(const char *line, char **env)
{
	char	**av;
	int		i;
	int		j;
	t_args	args;

	av = minisplit(line, ' ');
	args.ac = 0;
	i = 0;
	while (av[args.ac])
		args.ac++;
	ft_printf("ARGC: %i\n", args.ac);
	args.av = sanitizer(args.ac, av, env);
	i = 0;
	while (args.av[i])
	{
		j = 0;
		while (args.av[i][j])
		{
			write(1, &args.av[i][j], 1);
			j++;
		}
		write(1, " ", 1);
		i++;
	}
	write(1, "\n", 1);
	return (args);
}
