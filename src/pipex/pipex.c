/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:04:17 by plouda            #+#    #+#             */
/*   Updated: 2023/08/31 16:28:55 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler2(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("SIGNAL RECIEVED SIGINT\n");
		ft_printf("\n");
		exit (130);
	}
}

int	*reset_fds(int *in, int *out)
{
	int	*fd;

	fd = malloc(sizeof(int) * 2);
	*in = 0;
	*out = 1;
	return (fd);
}

int	pipex(t_list *cmds, char ***env, int *status, t_freebs stuff)
{
	t_pipe_variables	var;
	int					**fd;
	t_command			*cmd;

	var.i = 0;
	stuff.status = status;
	cmd = (t_command *)cmds->content;
	if (cmd->args != NULL && ft_lstsize(cmds) == 1 && is_b(cmd->args[0]))
		return (no_pipe(cmds, var, env, stuff), 0);
	fd = malloc(sizeof(int *) * ft_lstsize(cmds));
	stuff.fd = &fd;
	stuff.fd_n = ft_lstsize(cmds);
	while (cmds)
	{
		fd[var.i] = reset_fds(&var.in, &var.out);
		pipe(fd[var.i]);
		here_doc_check(cmds);
		forked_processes(var, stuff, fd, cmds);
		var.i++;
		cmds = cmds->next;
	}
	return (pipe_waiter(var, status, fd), 0);
}

void	waiter(t_list *cmds, int *status)
{
	t_command	*command;
	int			stat;

	command = (t_command *)cmds->content;
	while (cmds)
	{
		write(2, "Waiting...\n", 12);
		ft_putnbr_fd(command->pid, 2);
		waitpid(command->pid, &stat, 0);
		*status = WEXITSTATUS(stat);
		cmds = cmds->next;
	}
}
