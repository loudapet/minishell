/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 11:37:49 by plouda            #+#    #+#             */
/*   Updated: 2023/08/03 10:18:46 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Test: <infile1 << infile2 cmd args > outfile1 >outfile2 args2 >> outfile2 < infile3

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

void	get_files(int argc, char **argv, t_command *command)
{
	int	i;

	i = 0;
	while (i < argc)
	{
		if (argv[i][0] == '<')
		{
			if (!strncmp(argv[i], "<<", 2))
				here_doc(argv, i, command);
			else
				infile(argv, i, command);
		}
		if ((argv[i][0]) == '>')
		{
			if (!strncmp(argv[i], ">>", 2))
				append(argv, i, command);
			else
				outfile(argv, i, command);
		}
		i++;
	}
}

int	get_cmd_args_len(int argc, char **argv)
{
	int	i;
	int	len;

	i = 0;
	len = argc;
	while (i < argc)
	{
		if (argv[i][0] == '<' || argv[i][0] == '>')
		{
			if (ft_strlen(argv[i]) > 2)
				len--;
			else if (ft_strlen(argv[i]) == 2 && argv[i][1] != '>' && argv[i][1] != '<')
				len--;
			else
				len -= 2;
		}
		i++;
	}
	return (len);
}

char	**get_cmd_args(int argc, char **argv, char **argv_cpy)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < argc)
	{
		while (argv[i][0] == '<' || argv[i][0] == '>')
		{	
			if (ft_strlen(argv[i]) > 2)
				i++;
			else if (ft_strlen(argv[i]) == 2 && argv[i][1] != '>' && argv[i][1] != '<')
				i++;
			else
				i += 2;
			if (i >= argc)
				break ;
		}
		if (i >= argc)
			break ;
		argv_cpy[j] = argv[i];
		i++;
		j++;
	}
	argv_cpy[j] = NULL;
	return (argv_cpy);
}

t_command	command_redirection(int argc, char **argv)
{
	int		i;
	int		cmd_args_len;
	t_command	command;

	command.infile_path = NULL;
	command.outfile_path = NULL;
	command.infile_fd = 0;
	command.outfile_fd = 1;
	command.here_doc = 0;
	command.redirection = 0;
	cmd_args_len = get_cmd_args_len(argc, argv);
	command.cmd_args = malloc(sizeof(char *) * (cmd_args_len + 1));
	// handle malloc failure (how?)
	i = 0;
	while (i < argc)
	{
		get_files(argc, argv, &command);
		command.cmd_args = get_cmd_args(argc, argv, command.cmd_args);
		i++;
	}
	ft_printf("Infile: %s\nOutfile: %s\n", command.infile_path, command.outfile_path);
	ft_printf("Infile fd: %d\nOutfile fd: %d\n", command.infile_fd, command.outfile_fd);
	ft_printf("Redirection mode (0 no outfile, 1 truncate, 2 append): %d\n", command.redirection);
	ft_printf("here_doc status (0 none, 1 void, 2 infile): %d\n", command.here_doc);
	display_argv(command.cmd_args);
	return (command);
}
