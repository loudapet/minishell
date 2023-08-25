/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_remover.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 10:11:07 by plouda            #+#    #+#             */
/*   Updated: 2023/08/25 11:43:01 by plouda           ###   ########.fr       */
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

void	sanitize(t_sanitizer *san, char **av, int i, int *j)
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
