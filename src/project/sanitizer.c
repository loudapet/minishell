/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 10:11:07 by plouda            #+#    #+#             */
/*   Updated: 2023/08/11 10:27:19 by plouda           ###   ########.fr       */
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

void	index_checker(t_sanitizer *san, char **av, int i, int *j)
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

	// i = 0;
	k = 0;
	flag = 0;
	quote = 0;
	// while (av[i])
	// {
		// ft_printf("%s %d\n", av[i], has_redirection(av[i]));
	// 	if (has_redirection(av[i]))
	// 	{
			// if (has_redirection(av[i]) == 0)
			// 	temp = malloc(sizeof(char *) * (*ac + 1));
			// else
	// ft_printf("???%d %d\n", *ac, has_redirection(av[i]));
	temp = malloc(sizeof(char *) * (*ac + has_redirection(av[i])));
	// ft_printf("HAS_REDIRECTION %d AC %d\n", has_redirection(av[i]), *ac);
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
			// ft_printf("%s\n", temp[k]);
			temp[k] = NULL;
			j = 0;
			state = 0;
			while (av[i][j])
			{
				// ft_printf("%d %s %c\n", k, temp[k], av[i][j]);
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
				// ft_printf("WHY IS IT TWO %d %c %d\n", state, av[i][j], k);
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
				// ft_printf("%d %s %c\n", k, temp[k], av[i][j]);
				temp[k] = join(temp[k], av[i][j]);
				// ft_printf("%s %c %d %d\n", temp[k], av[i][j], k, state);
				j++;
			}
			k++;
			flag = 1;
		}
		free(av[i]);
		i++;
	}
	// ft_printf("AAAA%d\n", k);
	temp[k] = NULL;
	free(av);
	return (temp);
			// i = 0;
	// 	}
	// 	i++;
	// }
	// return (av);
}

char	**uuh(char **av, int *ac)
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

// jjj """j"'j'j j'j'j"" "" > should expand to jjj jjj jjj
char	**sanitizer(int *ac, char **av, char **env)
{
	int			i;
	//int			j;
	//t_sanitizer	san;

	i = 0;
	// ft_printf("%d",)
	while (av[i])
	{
		// ft_printf("%s\n", av[i]);
		i++;
	}
	i = 0;
	while (av[i] && *ac)
	{
		//san = reset_sanitizer();
		av[i] = expand_env(av[i], env);
		/* j = 0;
		while (av[i][j])
		{
			quote_counter((const char *)&av[i][j], \
				&san.quote, &san.single_quote);
			index_checker(&san, av, i, &j);
			j++;
		} */
		i++;
	}
	av[i] = NULL;
	av = uuh(av, ac);
	// ft_printf("%d\n", *ac);
	i = 0;
	while (av[i])
	{
		// ft_printf("%s\n", av[i]);
		i++;
	}
	return (av);
}
