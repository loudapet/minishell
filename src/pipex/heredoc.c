/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 09:36:45 by plouda            #+#    #+#             */
/*   Updated: 2023/08/30 11:23:01 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	heredoc_exec(t_command *command, int flag)
{
	int		*temp_pipe;
	char	*str;
	char	**delimiter;
	int		i;

	i = 0;
	temp_pipe = command->heredoc_pipe;
	delimiter = command->delimiter;
	while (command->here_doc_counter > 1 && delimiter[i + 1] != NULL )
	{
		write(STDOUT_FILENO, "> ", 2);
		str = get_next_line(STDIN_FILENO);
		if (!str)
			ft_printf("\nWarning: here-doc delimited by EOF, wanted %s\n", delimiter[i]);
		while (str && (ft_strncmp(delimiter[i], str, ft_strlen(delimiter[i]))  || ft_strlen(str) != ft_strlen(delimiter[i]) + 1))
		{
			write(STDOUT_FILENO, "> ", 2);
			free(str);
			str = get_next_line(STDIN_FILENO);
			if (!str)
				ft_printf("\nWarning: here-doc delimited by EOF, wanted %s\n", delimiter[i]);
		}
		i++;
		if (str)
			free(str);
	}
	write(STDOUT_FILENO, "> ", 2);
	str = get_next_line(STDIN_FILENO);
	if (!str)
		ft_printf("\n1Warning: here-doc delimited by EOF, wanted %s\n", delimiter[i]);
	while (str && (ft_strncmp(delimiter[i], str, ft_strlen(delimiter[i])) || ft_strlen(str) != ft_strlen(delimiter[i]) + 1))
	{
		write(STDOUT_FILENO, "> ", 2);
		if (flag == HERE_DOC_IN)
			write(temp_pipe[WRITE], str, ft_strlen(str));
		free(str);
		str = get_next_line(STDIN_FILENO);
		if (!str)
			ft_printf("\n2Warning: here-doc delimited by EOF, wanted %s\n", delimiter[i]);
	}
	if (str)
		free(str);
	//close(temp_pipe[WRITE]);
	return (temp_pipe[READ]);
}

void	heredoc_handler(t_list *cmds)
{
	int	i;
	t_command *command;

	i = 0;
	while (cmds)
	{
		command = (t_command *)cmds->content;
		if (command->here_doc)
		{
			if (command->here_doc == HERE_DOC_IN && !command->valid)
				command->here_doc = HERE_DOC_VOID;
			heredoc_exec(command, command->here_doc);
			close(command->heredoc_pipe[WRITE]);
			if (command->here_doc == HERE_DOC_VOID)
				close(command->heredoc_pipe[READ]);
		}
		else
		{
			close(command->heredoc_pipe[WRITE]);
			close(command->heredoc_pipe[READ]);
		}
		write(2, "Going next...\n", 15);
		cmds = cmds->next;
	}
}
