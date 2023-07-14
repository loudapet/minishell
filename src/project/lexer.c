/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:53:16 by plouda            #+#    #+#             */
/*   Updated: 2023/07/14 17:24:34 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*sanitize_single_quotes(const char *str)
{
	char	*str_san;
	int		i;
	int		j;
	int		quote_counter;

	i = 0;
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

char	**sanitizer(int ac, char **av)
{
	int	i;
	//int	j;
	//int	quote;
	//int	single_quote;
	char	**argv;

	i = 0;
	//quote = 0;
	//single_quote = 0;
	argv = malloc(sizeof(char **) * ac);
	while (av[i])
	{
		argv[i] = sanitize_single_quotes(av[i]);
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

void	lexer(const char *line)
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
	argv = sanitizer(ac, av);
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