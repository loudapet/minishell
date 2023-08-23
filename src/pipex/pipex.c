/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:04:17 by plouda            #+#    #+#             */
/*   Updated: 2023/08/23 11:26:16 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_exec(t_command *command, int flag)
{
	int		temp_pipe[2];
	char	*str;
	char	**delimiter;
	int		i;

	i = 0;
	pipe(temp_pipe);
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
		ft_printf("\nWarning: here-doc delimited by EOF, wanted %s\n", delimiter[i]);
	while (str && (ft_strncmp(delimiter[i], str, ft_strlen(delimiter[i])) || ft_strlen(str) != ft_strlen(delimiter[i]) + 1))
	{
		write(STDOUT_FILENO, "> ", 2);
		if (flag == HERE_DOC_IN)
			write(temp_pipe[WRITE], str, ft_strlen(str));
		free(str);
		str = get_next_line(STDIN_FILENO);
		if (!str)
			ft_printf("\nWarning: here-doc delimited by EOF, wanted %s\n", delimiter[i]);
	}
	if (str)
		free(str);
	close(temp_pipe[WRITE]);
	return (temp_pipe[READ]);
}

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

void	handler2(int sig)
{
	if (sig == SIGINT)
	{
		exit (130);
	}
}

void	pipex(t_list *cmds, char ***env, int *status)
{
	int		i;
	int		l;
	pid_t	pid;
	int		out;
	int		in;
	int		stdout;
	int		stat;
	int		**fd;
	t_command	*command;

	i = 0;
	command = (t_command *)cmds->content;
	if (ft_lstsize(cmds) == 1 && is_builtin(command->cmd_args[0]))
	{
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, handler2);
			if (command->here_doc)
				heredoc_exec(command, command->here_doc);
			exit (0);
		}
		else
		{
			waitpid(pid, &stat, 0);
			stdout = dup(STDOUT_FILENO);
			if (command->outfile_path != NULL)
			{
				if (command->redirection == 2)
					out = open(command->outfile_path, O_CREAT | O_WRONLY | O_APPEND, 00644);
				else
					out = open(command->outfile_path, O_CREAT | O_WRONLY | O_TRUNC, 00644);
				dup2(out, STDOUT_FILENO);
			}
			if (WEXITSTATUS(stat) == 130)
			{
				*status = WEXITSTATUS(stat);
				return ;
			}
			builtins(command->cmd_args, env, status);
			dup2(stdout, STDOUT_FILENO);
			return ;
		}
	}
	fd = malloc(sizeof(int*) * ft_lstsize(cmds));
	while (cmds)
	{
		in = 0;
		out = 1;
		fd[i] = malloc(sizeof(int) * 2);
		pipe(fd[i]);
		command = (t_command *)cmds->content;
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, handler2);
			if (i != 0)
				dup2(fd[i - 1][READ], STDIN_FILENO);
			if (cmds->next && (!command->here_doc || command->here_doc == HERE_DOC_VOID))
				dup2(fd[i][WRITE], STDOUT_FILENO);
			if (command->outfile_path != NULL)
			{
				if (command->redirection == 2)
					out = open(command->outfile_path, O_CREAT | O_WRONLY | O_APPEND, 00644);
				else
					out = open(command->outfile_path, O_CREAT | O_WRONLY | O_TRUNC, 00644);
				dup2(out, STDOUT_FILENO);
			}
			if (command->here_doc)
			{
				if (command->here_doc == HERE_DOC_IN)
				{

					in = heredoc_exec(command, command->here_doc);
					dup2(in, STDIN_FILENO);
					if (cmds->next)
						dup2(fd[i][WRITE], STDOUT_FILENO);

				}
				else
				{
					heredoc_exec(command, command->here_doc);
					if (cmds->next)
						dup2(fd[i][WRITE], STDOUT_FILENO);
				}
			}
			if (command->infile_path != NULL && (!command->here_doc || command->here_doc == HERE_DOC_VOID))
			{
				in = open(command->infile_path, O_RDONLY);
				if (in == -1)
				{
					ft_putstr_fd(" no such file or directory\n", 2);
					exit (1);
				}
				dup2(in, STDIN_FILENO);
			}
			builtins(command->cmd_args, env, status);
			exit(0);
		}
		else
			close(fd[i][WRITE]);
		i++;
		cmds = cmds->next;
	}
	l = 0;
	while (i > 0)
	{
		waitpid(pid, &stat, 0);
		*status = WEXITSTATUS(stat);
		i--;
		l++;
	}
	while (l >= 0)
	{
		free(fd[l]);
		l--;
	}
	free(fd);
}
