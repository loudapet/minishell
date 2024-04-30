/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 09:36:45 by plouda            #+#    #+#             */
/*   Updated: 2023/08/31 16:58:44 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	str_stuff(char **str, int mode, char *delimiter)
{
	if (mode == 0)
	{
		if (!(*str))
			ft_printf("\nWarning: here-doc delimited by EOF, wanted %s\n",
				delimiter);
	}
}

void	fake_heredoc(t_command *command, char *str, char **delimiter, int *i)
{
	*i = 0;
	while (command->here_doc_counter > 1 && delimiter[*i + 1] != NULL )
	{
		write(STDOUT_FILENO, "> ", 2);
		str = get_next_line(STDIN_FILENO);
		if (!str)
			ft_printf("\nWarning: here-doc delimited by EOF, wanted %s\n",
				delimiter[*i]);
		while (str && (ft_strncmp(delimiter[*i], str, ft_strlen(delimiter[*i]))
				|| ft_strlen(str) != ft_strlen(delimiter[*i]) + 1))
		{
			write(STDOUT_FILENO, "> ", 2);
			free(str);
			str = get_next_line(STDIN_FILENO);
			if (!str)
				ft_printf("\nWarning: here-doc delimited by EOF, wanted %s\n",
					delimiter[*i]);
		}
		(*i)++;
		if (str)
			free(str);
	}
}
