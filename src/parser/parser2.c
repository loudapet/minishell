/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehasalu <ehasalu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 11:37:49 by plouda            #+#    #+#             */
/*   Updated: 2023/09/01 17:00:20 by ehasalu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_argv(char **argv)
{
	int	i;
	int	j;

	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			write(1, &argv[i][j], 1);
			j++;
		}
		write(1, " ", 1);
		i++;
	}
	write(1, "\n", 1);
}

void	quote_remover(char **argv, int index)
{
	t_sanitizer	san;
	int			i;
	int			j;

	i = index;
	san = reset_sanitizer();
	j = 0;
	while (argv[i][j])
	{
		quote_counter((const char *)&argv[i][j], &san.quote, &san.single_quote);
		sanitize(&san, argv, i, &j);
		j++;
	}
}

void	get_files(int argc, char **argv, t_command *command, int *i)
{
	while (*i < argc)
	{
		if (argv[*i][0] == '<' && *i + 1 < argc)
		{
			quote_remover(argv, *i + 1);
			if (!strncmp(argv[*i], "<<", 2))
				here_doc(argv, *i, command);
			else
				infile(argv, *i, command);
			(*i)++;
		}
		else if ((argv[*i][0]) == '>' && *i + 1 < argc)
		{
			quote_remover(argv, *i + 1);
			if (!strncmp(argv[*i], ">>", 2))
				append(argv, *i, command);
			else
				outfile(argv, *i, command);
			(*i)++;
		}
		else if (argv[*i][0] == '|')
			break ;
		(*i)++;
	}
}
