/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slicer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 14:43:42 by plouda            #+#    #+#             */
/*   Updated: 2023/08/31 14:47:15 by plouda           ###   ########.fr       */
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
	while (ar[i])
	{
		if (ar[i] == '"')
		{
			if (quote == 0)
				quote = 2;
			else if (quote == 2)
				quote = 0;
		}
		if (ar[i] == '\'')
		{
			if (quote == 0)
				quote = 1;
			else if (quote == 1)
				quote = 0;
		}
		if ((ar[i] == '>' || ar[i] == '<' || ar[i] == '|') && quote == 0)
		{
			count++;
			if ((ar[i + 1] == '>' || ar[i + 1] == '<') && ar[i] != '|')
				i++;
		}
		i++;
	}
	if (count != 0)
		count = count * 2 + 1;
	if (ar[0] == '>' || ar[0] == '<' || ar[0] == '|')
		count--;
	if (ar[ft_strlen(ar) - 1] == '>' || ar[ft_strlen(ar) - 1] == '<' || ar[ft_strlen(ar) - 1] == '|')
		count--;
	i = 0;
	while (ar[i])
	{
		if (ar[i] == '|' && (ar[i + 1] == '>' || ar[i + 1] == '<'))
				count--;
		i++;
	}
	return (count);
}


char	**split_redirections(char **av, int *ac, int i)
{
		char	**temp;
	int		j;
	int		k;
	int		state;
	int		flag;
	int 	quote;

	k = 0;
	flag = 0;
	quote = 0;
	temp = malloc(sizeof(char *) * (*ac + has_redirection(av[i])));
	*ac += has_redirection(av[i]) - 1;
	i = 0;
	while (av[i])
	{
		if (has_redirection(av[i]) == 0 || flag == 1)
		{
			temp[k] = ft_strdup(av[i]);
			k++;
		}
		else
		{
			temp[k] = NULL;
			j = 0;
			state = 0;
			while (av[i][j])
			{
				if (av[i][j] == '\'' && quote == 0)
					quote = 1;
				else if (av[i][j] == '\'' && quote == 1)
					quote = 0;
				if (av[i][j] == '"' && quote == 0)
					quote = 2;
				else if (av[i][j] == '"' && quote == 2)
					quote = 0;
				if ((av[i][j] == '>' || av[i][j] == '<' || av[i][j] == '|') && state == 0)
					state = 1;
				else if (state == 0)
					state = 2;
				if ((av[i][j] == '>' || av[i][j] == '<' || av[i][j] == '|') && state == 2 && quote == 0)
				{
					k++;
					temp[k] = NULL;
					state = 1;
				}
				else if ((av[i][j] != '>' && av[i][j] != '<' && av[i][j] != '|') && state == 1)
				{
					k++;
					temp[k] = NULL;
					state = 2;
				}
				else if (av[i][j-1] == '|' && (av[i][j] == '>' || av[i][j] == '<') && state == 1)
				{
					k++;
					temp[k] = NULL;
					state = 1;
				}
				temp[k] = join(temp[k], av[i][j]);
				j++;
			}
			k++;
			flag = 1;
		}
		free(av[i]);
		i++;
	}
	temp[k] = NULL;
	free(av);
	return (temp);
}

char	**slice_redirections(char **av, int *ac)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (has_redirection(av[i]) != 0)
		{
			av = split_redirections(av, ac, i);
			i = 0;
			continue ;
		}
		i++;
	}
	return (av);
}
