/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:26:17 by plouda            #+#    #+#             */
/*   Updated: 2023/08/21 10:52:31 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// copying the initial part of the string before $ (marked by index)
static void	copy_base(char *str, char *str_exp, int *j, int index)
{
	while (*j < index)
	{
		str_exp[*j] = str[*j];
		(*j)++;
	}
}

// inserting value of the variable
static void	insert_var(char *str_exp, char *var_value, int *j)
{
	int	i;

	i = 0;
	while (var_value != NULL && var_value[i])
	{
		str_exp[*j] = var_value[i];
		i++;
		(*j)++;
	}
}

// inserting everything after variable until \0
static void	insert_rest(char *str, char *str_exp, int index, int j)
{
	while (str[index])
	{
		str_exp[j] = str[index];
		j++;
		index++;
	}
	str_exp[j] = '\0';
}

char	*expand_and_join(char *str, char *var_name, char *var_value, int index)
{
	int		j;
	int		fin_len;
	char	*str_exp;

	fin_len = ft_strlen(str) - (ft_strlen(var_name) + 1) + ft_strlen(var_value);
	str_exp = malloc(fin_len + 1);
	if (!str_exp)
		return (NULL);
	j = 0;
	copy_base(str, str_exp, &j, index);
	insert_var(str_exp, var_value, &j);
	index += ft_strlen(var_name) + 1;
	insert_rest(str, str_exp, index, j);
	free(str);
	return (str_exp);
}
