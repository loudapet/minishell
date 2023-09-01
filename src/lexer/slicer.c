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

void	split_redirection2(char **av, t_lexer_var *var, int *k, char **temp)
{
	if ((av[var->i][var->j] == '>' || av[var->i][var->j] == '<'
		|| av[var->i][var->j] == '|') && var->state == 2 && var->quote == 0)
	{
		(*k)++;
		temp[*k] = NULL;
		var->state = 1;
	}
	else if ((av[var->i][var->j] != '>' && av[var->i][var->j] != '<'
		&& av[var->i][var->j] != '|') && var->state == 1)
	{
		(*k)++;
		temp[*k] = NULL;
		var->state = 2;
	}
	else if (var->j != 0 && av[var->i][var->j - 1] == '|' 
		&& (av[var->i][var->j] == '>'
		|| av[var->i][var->j] == '<') && var->state == 1)
	{
		(*k)++;
		temp[*k] = NULL;
		var->state = 1;
	}
}

void	init_vars(t_lexer_var *var)
{
	var->k = 0;
	var->flag = 0;
	var->quote = 0;
	var->i = 0;
}

void	split_redirection_else(char **temp, char **av, t_lexer_var *var)
{
	temp[var->k] = NULL;
	var->j = 0;
	var->state = 0;
	while (av[var->i][var->j])
	{
		quote_changer(av, var->i, var->j, &var->quote);
		state_changer(av, var->i, var->j, &var->state);
		split_redirection2(av, var, &var->k, temp);
		temp[var->k] = join(temp[var->k], av[var->i][var->j]);
		var->j += 1;
	}
	var->k += 1;
	var->flag = 1;
}

char	**split_redirections(char **av, int *ac, int i)
{
	char		**temp;
	t_lexer_var	var;

	init_vars(&var);
	ft_printf("%d", has_redirection(av[i]));
	temp = malloc(sizeof(char *) * (*ac + has_redirection(av[i])));
	*ac += has_redirection(av[i]) - 1;
	while (av[var.i])
	{
		if (has_redirection(av[var.i]) == 0 || var.flag == 1)
		{
			temp[var.k] = ft_strdup(av[var.i]);
			var.k++;
		}
		else
		{
			split_redirection_else(temp, av, &var);
		}
		free(av[var.i]);
		var.i++;
	}
	temp[var.k] = NULL;
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
