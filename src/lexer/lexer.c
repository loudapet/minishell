/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:53:16 by plouda            #+#    #+#             */
/*   Updated: 2023/08/25 11:11:42 by plouda           ###   ########.fr       */
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

char	*get_env_var_name(char *str, int start)
{
	int		i;
	int		pos;
	char	*name;

	i = 0;
	pos = start + 1;
	while (ft_isalnum(str[pos])) // upgrade to any metacharacter
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

char	*expand_env(char *str, char **env, int status)
{
	int		i;
	char	*var_name;
	char	*var;
	int		quote;
	int		single_quote;

	i = 0;
	quote = 0;
	single_quote = 0;
	var_name = NULL;
	var = NULL;
	while (str[i])
	{
		quote_counter((const char *)&str[i], &quote, &single_quote);
		if (str[i] == '$' && str[i+1] != '?' 
			&& !ft_isalnum(str[i+1]) && !(single_quote % 2))
		{
			i++;
			continue ;
		}
		else if (str[i] == '$' && str[i+1] == '?' && !(single_quote % 2))
		{
			var = ft_itoa(status);
			str = expand_and_join(str, "?", var, i);
			i = -1;
			quote = 0;
			single_quote = 0;
			//ft_printf("String: %s\n", str);
			//free(var_name);
		}
		else if (str[i] == '$' && !(single_quote % 2))
		{
			var_name = get_env_var_name(str, i);
			var = get_env(var_name, env);
			str = expand_and_join(str, var_name, var, i);
			i = -1;
			quote = 0;
			single_quote = 0;
			free(var_name);
		}
		i++;
	}
	return (str);
}

// maybe consider redoing this for argv[i] != NULL
void	free_args(t_args args)
{
	int	i;

	i = 0;
	while (args.av[i])
	{
		free(args.av[i]);
		i++;
	}
	free(args.av);
}

// norm compliant, just delete helper output loops
t_args	lexer(const char *line, char **env, int status)
{
	char	**av;
	t_args	args;
	int			i;

	av = minisplit(line, ' ');
	args.ac = 0;
	while (av[args.ac])
		args.ac++;
	i = 0;
	while (av[i] && args.ac)
	{
		av[i] = expand_env(av[i], env, status);
		i++;
	}
	av[i] = NULL;
	args.av = slice_redirections(av, &args.ac);	
	return (args);
}
