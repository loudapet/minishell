/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:53:16 by plouda            #+#    #+#             */
/*   Updated: 2023/08/31 15:38:40 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_var_name(char *str, int start)
{
	int		i;
	int		pos;
	char	*name;

	i = 0;
	pos = start + 1;
	while (ft_isalnum(str[pos]))
	{
		i++;
		pos++;
	}
	name = malloc(sizeof(char) * i + 1);
	if (!name)
		return (NULL);
	i = 0;
	start++;
	while (start < pos)
	{
		name[i] = str[start];
		start++;
		i++;
	}
	name[i] = '\0';
	return (name);
}

char	*env_var(char *str, char **env, t_sanitizer *expand)
{
	char	*var;
	char	*var_name;

	var_name = get_env_var_name(str, expand->index);
	var = get_env(var_name, env);
	str = expand_and_join(str, var_name, var, expand->index);
	expand->index = -1;
	expand->quote = 0;
	expand->single_quote = 0;
	free(var_name);
	return (str);
}

char	*exit_status(char *str, int status, t_sanitizer *expand)
{
	char	*var;

	var = ft_itoa(status);
	str = expand_and_join(str, "?", var, expand->index);
	expand->index = -1;
	expand->quote = 0;
	expand->single_quote = 0;
	return (str);
}

char	*expand_env(char *str, char **env, int status)
{
	t_sanitizer	expand;

	expand.index = 0;
	expand.quote = 0;
	expand.single_quote = 0;
	while (str[expand.index])
	{
		quote_counter((const char *)&str[expand.index], \
		&expand.quote, &expand.single_quote);
		if (str[expand.index] == '$' && str[expand.index + 1] != '?'
			&& !ft_isalnum(str[expand.index + 1]) && !(expand.single_quote % 2))
		{
			expand.index++;
			continue ;
		}
		else if (str[expand.index] == '$' && str[expand.index + 1] == '?'
			&& !(expand.single_quote % 2))
			str = exit_status(str, status, &expand);
		else if (str[expand.index] == '$' && !(expand.single_quote % 2))
			str = env_var(str, env, &expand);
		expand.index++;
	}
	return (str);
}

t_args	lexer(const char *line, char **env, int status)
{
	char	**av;
	t_args	args;
	int		i;

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
