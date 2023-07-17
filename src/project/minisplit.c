/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minisplit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 13:29:48 by plouda            #+#    #+#             */
/*   Updated: 2023/07/14 14:26:34 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_word_count(const char *s, char c)
{
	unsigned int	i;
	unsigned int	flag;
	unsigned int	count;
	unsigned int	quote;
	unsigned int	single_quote;

	i = 0;
	flag = 0;
	count = 0;
	quote = 0;
	single_quote = 0;
	while (s[i] != 0)
	{
		if (s[i] == '"' && !(single_quote % 2))
			quote++;
		if (s[i] == '\'' && !(quote % 2))
			single_quote++;
		if (s[i] != c && flag == 0)
		{
			flag = 1;
			count++;
		}
		else if (s[i] == c)
		{
			if (!(quote % 2) && !(single_quote % 2))
			{
				flag = 0;
				quote = 0;
				single_quote = 0;
			}
		}
		//ft_printf("Quote: %i\n", quote);
		i++;
	}
	ft_printf("Words: %i\n", count);
	return (count);
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
	unsigned int	i;
	unsigned int	j;
	int				pos;
	int				quote;
	int				single_quote;

	i = 0;
	j = 0;
	quote = 0;
	single_quote = 0;
	pos = -1;
	while (i <= ft_strlen(s))
	{
		if (s[i] == '"' && !(single_quote % 2))
			quote++;
		if (s[i] == '\'' && !(quote % 2))
			single_quote++;
		if (s[i] != c && pos < 0)
			pos = i;
		else if ((s[i] == c || i == ft_strlen(s)) && pos >= 0)
		{
			if (!(quote % 2) && !(single_quote % 2))
			{
				split[j++] = ft_word(s, pos, i);
				pos = -1;
				quote = 0;
				single_quote = 0;
			}
		}
		i++;
	}
	split[j] = 0;
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
