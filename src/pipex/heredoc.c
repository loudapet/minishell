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

void	waithandler(int sig)
{
	if (sig == SIGUSR2)
	{
		g_signal = SIGUSR2;
		ft_printf("\n");
	}
	if (sig == SIGINT)
	{
		kill(0, SIGUSR2);
		exit(0);
	}
}

int	heredoc_exec(t_command *command, int flag)
{
	int		*temp_pipe;
	char	*str;
	char	**delimiter;
	int		i;

	str = NULL;
	temp_pipe = command->heredoc_pipe;
	delimiter = command->delimiter;
	fake_heredoc(command, str, delimiter, &i);
	write(STDOUT_FILENO, "> ", 2);
	str = get_next_line(STDIN_FILENO);
	str_stuff(&str, 0, delimiter[i]);
	while (str && (ft_strncmp(delimiter[i], str, ft_strlen(delimiter[i]))
			|| ft_strlen(str) != ft_strlen(delimiter[i]) + 1))
	{
		write(STDOUT_FILENO, "> ", 2);
		if (flag == HERE_DOC_IN)
			write(temp_pipe[WRITE], str, ft_strlen(str));
		free(str);
		str = get_next_line(STDIN_FILENO);
		str_stuff(&str, 0, delimiter[i]);
	}
	if (str)
		free(str);
	return (temp_pipe[READ]);
}

void	heredoc_pipes(t_list *cmds)
{
	t_command	*command;

	g_signal = 0;
	while (cmds)
	{
		command = (t_command *)cmds->content;
		if (command->here_doc)
		{
			if (command->here_doc == HERE_DOC_IN && !command->valid)
				command->here_doc = HERE_DOC_VOID;
			if (command->here_doc == HERE_DOC_VOID)
				close(command->heredoc_pipe[READ]);
		}
		else
		{
			close(command->heredoc_pipe[WRITE]);
			close(command->heredoc_pipe[READ]);
		}
		cmds = cmds->next;
	}
}

void	here_child(t_list *cmds)
{
	t_command	*command;

	signal(SIGINT, waithandler);
	signal(SIGUSR2, SIG_IGN);
	while (cmds)
	{
		command = (t_command *)cmds->content;
		if (command->here_doc == HERE_DOC_IN)
			close(command->heredoc_pipe[READ]);
		if (command->here_doc)
		{
			heredoc_exec(command, command->here_doc);
			close(command->heredoc_pipe[WRITE]);
		}
		cmds = cmds->next;
	}
	exit(0);
}

void	heredoc_handler(t_list *cmds)
{
	t_command	*command;
	pid_t		pid;

	heredoc_pipes(cmds);
	pid = fork();
	if (!pid)
		here_child(cmds);
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGUSR2, waithandler);
		waitpid(pid, NULL, 0);
		while (cmds)
		{
			command = (t_command *)cmds->content;
			if (command->here_doc)
				close(command->heredoc_pipe[WRITE]);
			cmds = cmds->next;
		}
	}
}
