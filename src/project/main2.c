/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehasalu <ehasalu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:15:35 by plouda            #+#    #+#             */
/*   Updated: 2023/09/01 15:53:53 by ehasalu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_thingy(char c, int *pipe)
{
	if (c == '>' || c == '<' || c == '|')
	{
		if (c == '|')
			*pipe = 1;
		else if (*pipe == 1)
		{
			*pipe = 0;
			return (0);
		}
		return (1);
	}
	return (0);
}

void	move_index(char *line, int *i)
{
	if (line[*i] == '>' && line[(*i) + 1] == '>')
		(*i)++;
	else if (line[*i] == '<' && line[(*i) + 1] == '<')
		(*i)++;
}

int	space_mover_pipe_checker(char *line, int *i, int *quote, int *pipe)
{
	*i = 0;
	*quote = 0;
	*pipe = 0;
	while (line[*i] == ' ')
		i++;
	if (line[*i] == '|')
	{
		ft_putstr_fd("Invalid syntax\n", 2);
		return (0);
	}
	return (1);
}

void	quote_state(char *line, int *quote, int i)
{
	if (line[i] == '\'' && *quote == 0)
		*quote = 1;
	else if (line[i] == '"' && *quote == 0)
		*quote = 2;
	else if (line[i] == '\'' && *quote == 1)
		*quote = 0;
	else if (line[i] == '"' && *quote == 2)
		*quote = 0;
}

void	innit_loop(t_prompt_variables *pr_var, char **env)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	pr_var->i = 0;
	pr_var->hostname = get_hostname();
	pr_var->dir = get_directory(env);
	pr_var->specs = ft_strjoin(pr_var->username, pr_var->hostname);
	pr_var->prompt = ft_strjoin(pr_var->specs, pr_var->dir);
	pr_var->line = readline((const char *)pr_var->prompt);
}
