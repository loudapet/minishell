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

void	no_pipe(t_list *cmds, t_pipe_variables var, char ***env, t_freebs stuff)
{
	int					norm_stdout;
	t_command			*cmd;

	cmd = (t_command *)cmds->content;
	signal(SIGINT, SIG_IGN);
	norm_stdout = dup(STDOUT_FILENO);
	if (cmd->outfile_path != NULL)
	{
		if (cmd->redirection == 2)
			var.out = open(cmd->outfile_path, 
					O_CREAT | O_WRONLY | O_APPEND, 00644);
		else
			var.out = open(cmd->outfile_path, 
					O_CREAT | O_WRONLY | O_TRUNC, 00644);
		dup2(var.out, STDOUT_FILENO);
	}
	if (cmd->args != NULL)
		builtins(cmd->args, env, stuff.status, stuff);
	dup2(norm_stdout, STDOUT_FILENO);
}

void	pipe_child(t_pipe_variables var, 
	t_freebs st, int **fd, t_list *cmds)
{
	t_command	*cmd;

	cmd = (t_command *)cmds->content;
	close(fd[var.i][READ]);
	signal(SIGINT, handler2);
	if (var.i != 0 && (cmd->here_doc || (cmd->infile_path 
				!= NULL && cmd->valid)))
		close(fd[var.i - 1][READ]);
	if (var.i != 0 && (!cmd->here_doc 
			|| cmd->here_doc == HERE_DOC_VOID))
		dup2(fd[var.i - 1][READ], STDIN_FILENO);
	if (cmds->next)
		dup2(fd[var.i][WRITE], STDOUT_FILENO);
	if (cmd->here_doc == HERE_DOC_IN)
		dup2(cmd->heredoc_pipe[READ], STDIN_FILENO);
	if (cmd->outfile_path != NULL)
		dup_outfile(cmd, var, st, fd);
	if (cmd->infile_path != NULL && (!cmd->here_doc 
			|| cmd->here_doc == HERE_DOC_VOID))
		dup_infile(cmd, var, st, fd);
	if (var.i != 0)
		st.fd_n = var.i;
	builtins(cmd->args, st.env, st.status, st);
	free_stuff(st, var.i);
	exit(*(st.status));
}

void	pipe_parent(t_list *cmds, t_pipe_variables var, int **fd)
{
	t_command	*cmd;

	cmd = (t_command *)cmds->content;
	close(fd[var.i][WRITE]);
	signal(SIGINT, SIG_IGN);
	if (var.i != 0)
		close(fd[var.i - 1][READ]);
	if (cmd->here_doc == HERE_DOC_IN)
		close(cmd->heredoc_pipe[READ]);
}

void	pipe_waiter(t_pipe_variables var, int *status, int **fd)
{
	var.l = 0;
	while (var.i > 0)
	{
		write(2, "Waiting...\n", 12);
		waitpid(-1, &(var.stat), 0);
		*status = WEXITSTATUS(var.stat);
		var.i--;
		var.l++;
	}
	while (var.i < var.l)
	{
		free(fd[var.i]);
		var.i++;
	}
	free(fd);
}

void	forked_processes(t_pipe_variables var, 
	t_freebs stuff, int **fd, t_list *cmds)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		pipe_child(var, stuff, fd, cmds);
	else
		pipe_parent(cmds, var, fd);
}
