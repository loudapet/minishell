/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehasalu <ehasalu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 11:37:49 by plouda            #+#    #+#             */
/*   Updated: 2023/09/01 16:59:34 by ehasalu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Test: <infile1 << infile2 cmd args >
//outfile1 >outfile2 args2 >> outfile2 < infile3

int	get_cmd_args_len(int argc, char **argv, int *index)
{
	int	i;
	int	len;

	i = *index;
	len = 0;
	while (i < argc)
	{
		len++;
		if (argv[i][0] == '<' || argv[i][0] == '>' || argv[i][0] == '|')
		{
			if (argv[i][0] == '<' || argv[i][0] == '>')
				len -= 2;
			else if (argv[i][0] == '|')
			{
				len--;
				break ;
			}
		}
		i++;
	}
	return (len);
}

void	get_cmd_args_2(char **argv, int argc, int *i, int *flag)
{
	while (argv[*i][0] == '<' || argv[*i][0] == '>' || argv[*i][0] == '|')
	{
		if (argv[*i][0] == '<' || argv[*i][0] == '>')
			*i += 2;
		else if (argv[*i][0] == '|')
		{
			*flag = 1;
			(*i)++;
			break ;
		}
		else
			*i += 2;
		if (*i >= argc)
			break ;
	}
}

char	**get_cmd_args(int argc, char **argv, char **argv_cpy, int i)
{
	int	j;
	int	flag;

	flag = 0;
	j = 0;
	while (i < argc && argv != NULL)
	{
		get_cmd_args_2(argv, argc, &i, &flag);
		if (i >= argc || flag)
			break ;
		argv_cpy[j] = ft_strdup(argv[i]);
		quote_remover(argv_cpy, j);
		i++;
		j++;
	}
	if (argv != NULL)
		argv_cpy[j] = NULL;
	return (argv_cpy);
}

// What happens when there is just a redirection, but no command?

void	command_init(t_command *command)
{
	command->infile_path = NULL;
	command->outfile_path = NULL;
	command->infile_fd = 0;
	command->outfile_fd = 1;
	command->here_doc = 0;
	command->here_doc_counter = 0;
	command->redirection = 0;
	command->delimiter = NULL;
	command->valid = 1;
	command->pid = 0;
}

t_command	*parser(int argc, char **argv, int *i)
{
	int			cmd_args_len;
	t_command	*command;

	command = malloc(sizeof(t_command));
	command_init(command);
	pipe(command->heredoc_pipe);
	cmd_args_len = get_cmd_args_len(argc, argv, i);
	command->args = malloc(sizeof(char *) * (cmd_args_len + 1));
	if (cmd_args_len == 0)
	{
		free(command->args);
		command->args = NULL;
	}
	else
		command->args = get_cmd_args(argc, argv, command->args, *i);
	get_files(argc, argv, command, i);
	(*i)++;
	if (*i > argc)
		*i = argc;
	return (command);
}
