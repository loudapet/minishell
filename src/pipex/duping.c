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

void	dup_outfile(t_command *cmd, t_pipe_variables var, t_freebs st, int **fd)
{
	if (cmd->redirection == APPEND)
		var.out = open(cmd->outfile_path, O_CREAT 
				| O_WRONLY | O_APPEND, 00644);
	else
		var.out = open(cmd->outfile_path, O_CREAT 
				| O_WRONLY | O_TRUNC, 00644);
	if (var.out < 0)
	{
		perror(cmd->outfile_path);
		close(fd[var.i][READ]);
		close(cmd->heredoc_pipe[READ]);
		write(2, "msh: ", 5);
		free_stuff(st, var.i);
		exit(1);
	}
	dup2(var.out, STDOUT_FILENO);
}

void	dup_infile(t_command *cmd, t_pipe_variables var, t_freebs st, int **fd)
{
	var.in = open(cmd->infile_path, O_RDONLY);
	if (var.in < 0)
	{
		perror(cmd->infile_path);
		close(cmd->heredoc_pipe[READ]);
		close(fd[var.i][READ]);
		write(2, "msh: ", 5);
		free_stuff(st, var.i);
		exit(1);
	}
	dup2(var.in, STDIN_FILENO);
}
