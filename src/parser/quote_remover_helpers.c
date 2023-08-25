/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitizer_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:24:43 by plouda            #+#    #+#             */
/*   Updated: 2023/07/25 14:30:37 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_front(t_quotes *quotes, int index, const char *str)
{
	while (quotes->k < index)
	{
		quotes->str_san[quotes->j] = str[quotes->k];
		quotes->k++;
		quotes->j++;
	}
}

void	copy_rest(t_quotes *quotes, const char *str, char type)
{
	while (str[quotes->i])
	{
		while (str[quotes->i] == type && quotes->quote_counter != 2)
		{
			quotes->quote_counter++;
			quotes->i++;
		}
		if (!str[quotes->i])
			break ;
		quotes->str_san[quotes->j] = str[quotes->i];
		quotes->i++;
		quotes->j++;
	}
}

t_quotes	init_quote_vars(const char *str, int index)
{
	t_quotes	quotes;

	quotes.i = index;
	quotes.j = 0;
	quotes.k = 0;
	quotes.quote_counter = 0;
	quotes.str_san = malloc(sizeof(char *) * (ft_strlen(str) - 2 + 1));
	if (!quotes.str_san)
		quotes.str_san = NULL;
	return (quotes);
}

t_sanitizer	reset_sanitizer(void)
{
	t_sanitizer	sanitizer;

	sanitizer.index = 0;
	sanitizer.quote = 0;
	sanitizer.single_quote = 0;
	return (sanitizer);
}

void	quote_counter(const char *c, int *quote, int *single_quote)
{
	if (*c == '\'' && !(*quote % 2))
		(*single_quote)++;
	else if (*c == '"' && !(*single_quote % 2))
		(*quote)++;
}
