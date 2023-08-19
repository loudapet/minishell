/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:04:17 by plouda            #+#    #+#             */
/*   Updated: 2023/08/14 09:32:25 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *com)
{
	if (!ft_strncmp(com, "echo", 5))
		return (1);
	else if (!ft_strncmp(com, "cd", 3))
		return (1);
	else if (!ft_strncmp(com, "export", 7))
		return (1);
	else if (!ft_strncmp(com, "unset", 6))
		return (1);
	else if (!ft_strncmp(com, "env", 4))
		return (1);
	else if (!ft_strncmp(com, "pwd", 4))
		return (1);
	else if (!ft_strncmp(com, "exit", 5))
		return (1);
	return (0);
}

void	pipex(t_list *cmds, char ***env, int *status)
{
	int		i;
	pid_t	pid;
	int		out;
	int		in;
	int		stdout;
	int		stat;
	int		**fd;//[ft_lstsize(cmds)][2];
	t_command	*command;

	fd = malloc(sizeof(int*) * ft_lstsize(cmds));
	i = 0;
	command = (t_command *)cmds->content;
	if (ft_lstsize(cmds) == 1 && is_builtin(command->cmd_args[0]))
	{
		stdout = dup(1);
		if (command->outfile_path != NULL)
		{
			if (command->redirection == 2)
				out = open(command->outfile_path, O_CREAT | O_WRONLY | O_APPEND, 00644);
			else
				out = open(command->outfile_path, O_CREAT | O_WRONLY | O_TRUNC, 00644);
			dup2(out, 1);
		}
		builtins(command->cmd_args, env, status);
		dup2(stdout, 1);
		return ;
	}
	while (cmds)
	{
		fd[i] = malloc(sizeof(int) * 2);
		pipe(fd[i]);
		command = (t_command *)cmds->content;
		pid = fork();
		if (pid == 0)
		{
			if (i != 0)
				dup2(fd[i - 1][0], 0);
			if (cmds->next)
				dup2(fd[i][1], 1);
			if (command->outfile_path != NULL)
			{
				if (command->redirection == 2)
					out = open(command->outfile_path, O_CREAT | O_WRONLY | O_APPEND, 00644);
				else
					out = open(command->outfile_path, O_CREAT | O_WRONLY | O_TRUNC, 00644);
				dup2(out, 1);
			}
			if (command->infile_path != NULL)
			{
				in = open(command->infile_path, O_RDONLY);
				if (in == -1)
				{
					ft_putstr_fd(" no such file or directory\n", 2);
					exit (1);
				}
				dup2(in, 0);
			}
			builtins(command->cmd_args, env, status);
			exit(0);
		}
		else
			close(fd[i][1]);
		i++;
		cmds = cmds->next;
	}
	while (i > 0)
	{
		waitpid(pid, &stat, 0);
		*status = WEXITSTATUS(stat);
		i--;
	}
}
