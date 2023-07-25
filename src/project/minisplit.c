/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minisplit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 13:29:48 by plouda            #+#    #+#             */
/*   Updated: 2023/07/25 14:26:28 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_minisplit	init_minisplit(void)
{
	t_minisplit	minisplit;

	minisplit.i = 0;
	minisplit.j = 0;
	minisplit.flag = 0;
	minisplit.count = 0;
	minisplit.quote = 0;
	minisplit.single_quote = 0;
	minisplit.pos = -1;
	return (minisplit);
}

static int	ft_word_count(const char *s, char c)
{
	t_minisplit	msplit;

	msplit = init_minisplit();
	while (s[msplit.i] != 0)
	{
		quote_counter(&s[msplit.i], &msplit.quote, &msplit.single_quote);
		if (s[msplit.i] != c && msplit.flag == 0)
		{
			msplit.flag = 1;
			msplit.count++;
		}
		else if (s[msplit.i] == c)
		{
			if (!(msplit.quote % 2) && !(msplit.single_quote % 2))
			{
				msplit.flag = 0;
				msplit.quote = 0;
				msplit.single_quote = 0;
			}
		}
		msplit.i++;
	}
	return (msplit.count);
}

static char	*ft_word(const char *s, unsigned int start, unsigned int end)
{
	char			*word;
	unsigned int	i;

	i = 0;
	word = malloc((end - start + 1) * sizeof(char));
	while (start < end)
	{
		word[i] = s[start];
		i++;
		start++;
	}
	word[i] = 0;
	return (word);
}

static char	**ft_fill(char **split, char const *s, char c)
{
	t_minisplit	msplit;

	msplit = init_minisplit();
	while (msplit.i <= (int)ft_strlen(s))
	{
		quote_counter(&s[msplit.i], &msplit.quote, &msplit.single_quote);
		if (s[msplit.i] != c && msplit.pos < 0)
			msplit.pos = msplit.i;
		else if ((s[msplit.i] == c || msplit.i == (int)ft_strlen(s))
			&& msplit.pos >= 0)
		{
			if (!(msplit.quote % 2) && !(msplit.single_quote % 2))
			{
				split[msplit.j++] = ft_word(s, msplit.pos, msplit.i);
				msplit.pos = -1;
				msplit.quote = 0;
				msplit.single_quote = 0;
			}
		}
		msplit.i++;
	}
	split[msplit.j] = 0;
	return (split);
}

char	**minisplit(char const *s, char c)
{
	char			**split;

	if (!s)
		return (NULL);
	split = malloc((ft_word_count(s, c) + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	return (ft_fill(split, s, c));
}
