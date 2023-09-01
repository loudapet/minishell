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

void	quote_changer(char **av, int i, int j, int *quote)
{
	if (av[i][j] == '\'' && *quote == 0)
		*quote = 1;
	else if (av[i][j] == '\'' && *quote == 1)
		*quote = 0;
	if (av[i][j] == '"' && *quote == 0)
		*quote = 2;
	else if (av[i][j] == '"' && *quote == 2)
		*quote = 0;

}

void	state_changer(char **av, int i, int j, int *state)
{
	if ((av[i][j] == '>' || av[i][j] == '<'
		|| av[i][j] == '|') && *state == 0)
		*state = 1;
	else if (*state == 0)
		*state = 2;
}

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
	else if (av[var->i][var->j - 1] == '|' && (av[var->i][var->j] == '>'
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
			// split_redirection_else(temp, av, &var);
			temp[var.k] = NULL;
			var.j = 0;
			var.state = 0;
			while (av[var.i][var.j])
			{
				quote_changer(av, var.i, var.j, &var.quote);
				state_changer(av, var.i, var.j, &var.state);
				split_redirection2(av, &var, &var.k, temp);
				temp[var.k] = join(temp[var.k], av[var.i][var.j]);
				var.j++;
			}
			var.k++;
			var.flag = 1;
		}
		free(av[var.i]);
		i++;
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
