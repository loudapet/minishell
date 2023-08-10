/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 10:11:07 by plouda            #+#    #+#             */
/*   Updated: 2023/07/25 14:29:57 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*sanitize_quotes(const char *str, int index, char type)
{
	t_quotes	quotes;

	quotes = init_quote_vars(str, index);
	copy_front(&quotes, index, str);
	copy_rest(&quotes, str, type);
	quotes.str_san[quotes.j] = '\0';
	free((char *)str);
	return (quotes.str_san);
}

void	index_checker(t_sanitizer *san, char **av, int i, int *j)
{
	if (san->quote == 2)
	{
		av[i] = sanitize_quotes(av[i], san->index, '"');
		*j -= 2;
		san->index = *j + 1;
		if (*j < 0)
			san->index = 0;
		san->quote = 0;
	}
	if (san->single_quote == 2)
	{
		av[i] = sanitize_quotes(av[i], san->index, '\'');
		*j -= 2;
		san->index = *j + 1;
		if (*j < 0)
			san->index = 0;
		san->single_quote = 0;
	}
}

int	count_something_i_forgot(char *av)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (av[i])
	{
		if ((av[i] == '<' && (i == 0 || av[i - 1] != '<'))
			|| (av[i] == '>' && (i == 0 || av[i - 1] != '>')))
			k++;
		i++;
	}
	return (k);
}

char	*quote_redi_diffetiator(char *av)
{
	int		i;
	int		j;
	char	*temp;

	j = 0;
	i = 0;
	if (av[i] == '\'' || av[i] == '"')
		return (av);
	temp = malloc(ft_strlen(av) + count_something_i_forgot(av) + 1);
	while (av[i])
	{
		if ((av[i] == '<' && (i == 0 || av[i - 1] != '<'))
			|| (av[i] == '>' && (i == 0 || av[i - 1] != '>')))
		{
			temp[j] = '\\';
			j++;
		}
		temp[j] = av[i];
		j++;
		i++;
	}
	temp[j] = '\0';
	free(av);
	return (temp);
}

// jjj """j"'j'j j'j'j"" "" > should expand to jjj jjj jjj
char	**sanitizer(int ac, char **av, char **env)
{
	int			i;
	int			j;
	t_sanitizer	san;

	i = 0;
	while (av[i])
	{
		av[i] = quote_redi_diffetiator(av[i]);
		i++;
	}
	i = 0;
	while (av[i] && ac)
	{
		san = reset_sanitizer();
		av[i] = expand_env(av[i], env);
		j = 0;
		while (av[i][j])
		{
			quote_counter((const char *)&av[i][j], \
				&san.quote, &san.single_quote);
			index_checker(&san, av, i, &j);
			j++;
		}
		i++;
	}
	av[i] = NULL;
	av = backslash_splitter(av);
	return (av);
}
