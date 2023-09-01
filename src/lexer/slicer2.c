/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slicer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehasalu <ehasalu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 14:43:42 by plouda            #+#    #+#             */
/*   Updated: 2023/09/01 17:49:52 by ehasalu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join(char *original, char c)
{
	char	*temp;
	int		i;

	if (original == NULL)
	{
		temp = malloc(2);
		temp[0] = c;
		temp[1] = '\0';
		return (temp);
	}
	i = 0;
	temp = malloc(ft_strlen(original) + 2);
	while (original[i])
	{
		temp[i] = original[i];
		i++;
	}
	temp[i] = c;
	temp[i + 1] = '\0';
	free (original);
	return (temp);
}

void	has_redirection_loop(char *ar, int *quote, int *i, int *count)
{
	while (ar[*i])
	{
		if (ar[*i] == '"')
		{
			if (*quote == 0)
				*quote = 2;
			else if (*quote == 2)
				*quote = 0;
		}
		if (ar[*i] == '\'')
		{
			if (*quote == 0)
				*quote = 1;
			else if (*quote == 1)
				*quote = 0;
		}
		if ((ar[*i] == '>' || ar[*i] == '<' || ar[*i] == '|') && *quote == 0)
		{
			(*count)++;
			if ((ar[*i + 1] == '>' || ar[*i + 1] == '<') && ar[*i] != '|')
				(*i)++;
		}
		(*i)++;
	}
}

void	has_redirection_end(char *ar, int *count)
{
	int	i;

	i = 0;
	while (ar[i])
	{
		if (ar[i] == '|' && (ar[i + 1] == '>' || ar[i + 1] == '<'))
			(*count)--;
		i++;
	}
}

int	has_redirection(char *ar)
{
	int	i;
	int	quote;
	int	count;

	count = 0;
	quote = 0;
	i = 0;
	if (!ft_strncmp(ar, ">>", 3) || !ft_strncmp(ar, "<<", 3)
		|| !ft_strncmp(ar, "<", 2) || !ft_strncmp(ar, ">", 2)
		|| !ft_strncmp(ar, "|", 2))
		return (0);
	has_redirection_loop(ar, &quote, &i, &count);
	if (count != 0)
		count = count * 2 + 1;
	if (ar[0] == '>' || ar[0] == '<' || ar[0] == '|')
		count--;
	if (ar[ft_strlen(ar) - 1] == '>' || ar[ft_strlen(ar) - 1] == '<'
		|| ar[ft_strlen(ar) - 1] == '|')
		count--;
	has_redirection_end(ar, &count);
	return (count);
}
