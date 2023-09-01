/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_checkers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehasalu <ehasalu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 15:45:14 by ehasalu           #+#    #+#             */
/*   Updated: 2023/09/01 15:54:12 by ehasalu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

int	error_returner(void)
{
	ft_putstr_fd("Invalid syntax\n", 2);
	return (0);
}

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

int	syn_ch(char *line)
{
	int	i;
	int	quote;
	int	pipe;

	if (!space_mover_pipe_checker(line, &i, &quote, &pipe))
		return (0);
	while (line[i])
	{
		quote_state(line, &quote, i);
		if (quote == 0 && is_thingy(line[i], &pipe))
		{
			move_index(line, &i);
			i++;
			while (line[i] == ' ')
				i++;
			if (is_thingy(line[i], &pipe) || line[i] == '\0')
				return (error_returner());
		}
		i++;
	}
	return (1);
}

int	quot_ch(char *line)
{
	int	i;
	int	quote;

	quote = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' && quote == 0)
			quote = 1;
		else if (line[i] == '"' && quote == 0)
			quote = 2;
		else if (line[i] == '\'' && quote == 1)
			quote = 0;
		else if (line[i] == '"' && quote == 2)
			quote = 0;
		i++;
	}
	if (quote != 0)
	{
		ft_putstr_fd("Close your quotes! >:C\n", 2);
		return (0);
	}
	return (1);
}
