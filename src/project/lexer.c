/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:53:16 by plouda            #+#    #+#             */
/*   Updated: 2023/07/19 14:08:04 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_var_name(char *str, int start)
{
	int	i;
	int pos;
	char *name;

	i = 0;
	pos = start + 1;
	while (str[pos] != '\0' && str[pos] != ' ' && str[pos] != '$'
			&& str[pos] != '"' && str[pos] != '\'') // upgrade to any metacharacter
	{
		i++;
		pos++;
	}
	name = malloc(sizeof(char) * i + 1);
	if (!name)
		return (NULL);
	i = 0;
	start++; // no $ needed anymore
	while (start < pos)
	{
		name[i] = str[start];
		start++;
		i++;
	}
	name[i] = '\0';
	return (name);
}

void	display_env(char **env)
{
	int	i;
	int	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j])
		{
			write(1, &env[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}

char	*expand_and_join(char *str, char *var_name, char *var_value, int index)
{
	int	i;
	int	j;
	int	fin_len;
	char	*str_exp;

	fin_len = ft_strlen(str) - (ft_strlen(var_name) + 1) + ft_strlen(var_value);
	//ft_printf("Final length: %i\n", fin_len);
	str_exp = malloc(fin_len + 1); // protecc
	j = 0;
	// copying until $
	while (j < index)
	{
		str_exp[j] = str[j];
		j++;
	}
	// importing value of the variable
	i = 0;
	while (var_value != NULL && var_value[i])
	{
		str_exp[j] = var_value[i];
		i++;
		j++;
	}
	// shifting pointer of the original string to the first char behind the variable name, then copying until 0
	index += ft_strlen(var_name) + 1;
	while (str[index])
	{
		str_exp[j] = str[index];
		j++;
		index++;
	}
	str_exp[j] = '\0';
	/* free(var_name);
	free(var_value);
	free(str); */
	return (str_exp);
}

char	*expand_env(char *str, char **env)
{
	int		i;
	char	*var_name;
	char	*var;
	int		quote;
	int		single_quote;

	i = 0;
	quote = 0;
	single_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !(quote % 2))
			single_quote++;
		if (str[i] == '"' && !(single_quote % 2))
			quote++;
		if (str[i] == '$' && !(single_quote % 2))
		{
			var_name = get_env_var_name(str, i);
			//ft_printf("Variable name: %s\n", var_name);
			var = get_env(var_name, env);
			//ft_printf("Expanded variable: %s\n", var);
			str = expand_and_join(str, var_name, var, i);
			i = -1;
		}
		i++;
	}
	return (str);
}

char	*sanitize_double_quotes(const char *str, int index)
{
	char	*str_san;
	int		i;
	int		j;
	int		k;
	int		quote_counter;

	quote_counter = 0;
	i = index;
	j = 0;
	k = 0;
	str_san = malloc(sizeof(char *) * (ft_strlen(str) - 2 + 1)); // protecc
	while (k < index)
	{
		str_san[j] = str[k];
		k++;
		j++;
	}
	while (str[i])
	{
		while (str[i] == '"' && quote_counter != 2)
		{
			quote_counter++;
			i++;
		}
		if (!str[i])
			break;
		str_san[j] = str[i];
		i++;
		j++;
	}
	str_san[j] = '\0';
	free((char *)str);
	return (str_san);
}

char	*sanitize_single_quotes(const char *str, int index)
{
	char	*str_san;
	int		i;
	int		j;
	int		k;
	int		quote_counter;

	quote_counter = 0;
	i = index;
	str_san = malloc(sizeof(char *) * (ft_strlen(str) - 2 + 1)); // protecc
	j = 0;
	k = 0;
	while (k < index)
	{
		str_san[j] = str[k];
		k++;
		j++;
	}
	while (str[i])
	{
		while (str[i] == '\'' && quote_counter != 2)
		{
			quote_counter++;
			i++;
		}
		if (!str[i])
			break;
		str_san[j] = str[i];
		i++;
		j++;
	}
	str_san[j] = '\0';
	free((char *)str);
	return (str_san);
}

// jjj """j"'j'j j'j'j"" "" > should expand to jjj jjj jjj
char	**sanitizer(int ac, char **av, char **env)
{
	int	i;
	int	j;
	char	**argv;
	int	quote;
	int	single_quote;
	int	index;

	i = 0;
	argv = malloc(sizeof(char *) * ac);
	while (av[i])
	{
		index = 0;
		quote = 0;
		single_quote = 0;
		av[i] = expand_env(av[i], env);
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] == '\'' && !(quote % 2))
				single_quote++;
			else if (av[i][j] == '"' && !(single_quote % 2))
				quote++;
			if (quote == 2)
			{
				av[i] = sanitize_double_quotes(av[i], index);
				j -= 2;
				index = j + 1;
				if (j < 0)
					index = 0;
				quote = 0;
			}
			if (single_quote == 2)
			{
				av[i] = sanitize_single_quotes(av[i], index);
				j -= 2;
				index = j + 1;
				if (j < 0)
					index = 0;
				single_quote = 0;
			}
			j++;
		}
		//ft_printf("SANITIZING...\n");
		argv[i] = ft_strdup(av[i]);
		free(av[i]);
		i++;
	}
	argv[i] = NULL;
	free(av);
	return (argv);
}

void	lexer(const char *line, char **env)
{
	char	**av;
	int		ac;
	int		i;
	int		j;
	char	**argv;

	av = minisplit(line, ' ');
	ac = 0;
	while (av[ac])
	{
		i = 0;
		while (av[ac][i])
		{
			write(1, &av[ac][i], 1);
			i++;
		}
		write(1, "\n", 1);
		ac++;
	}
	ac++;
	ft_printf("-----------EXPANDING AND SANITIZING----------- \n");
	//display_env(env);
	argv = sanitizer(ac, av, env);
	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			write(1, &argv[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}