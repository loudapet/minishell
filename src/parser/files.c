/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 11:44:04 by plouda            #+#    #+#             */
/*   Updated: 2023/08/29 11:38:12 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Should probably not pass a whole stucture if it can be avoided

void	here_doc(char **argv, int i, t_command *command)
{
	int		j;
	char	**delimiter_cpy;

	command->here_doc = HERE_DOC_IN;
	if (!command->valid)
		command->here_doc = HERE_DOC_VOID;
	delimiter_cpy = malloc(sizeof(char *) * (command->here_doc_counter + 1));
	j = 0;
	while (command->delimiter != NULL && j < command->here_doc_counter)
	{
		delimiter_cpy[j] = ft_strdup(command->delimiter[j]);
		j++;
	}
	delimiter_cpy[j] = NULL;
	command->here_doc_counter++;
	j = 0;
	if (command->delimiter != NULL)
	{
		while (command->delimiter[j] != NULL)
		{
			free(command->delimiter[j]);
			j++;
		}
		free(command->delimiter[j]);
		free(command->delimiter);
	}
	command->delimiter = malloc(sizeof(char *)
			* (command->here_doc_counter + 1));
	j = 0;
	while (delimiter_cpy[j] != NULL)
	{
		command->delimiter[j] = ft_strdup(delimiter_cpy[j]);
		free(delimiter_cpy[j]);
		j++;
	}
	free(delimiter_cpy);
	command->delimiter[j++] = ft_strdup(argv[i + 1]);
	command->delimiter[j] = NULL;
}

void	infile(char **argv, int i, t_command *command)
{
	if (command->valid)
	{
		if (command->infile_path != NULL)
			free(command->infile_path);
		if (ft_strlen(argv[i]) == 1 && argv[i][0] == '<')
			command->infile_path = ft_strdup(argv[i + 1]);
		command->infile_fd = open(command->infile_path, O_RDONLY);
		if (command->infile_fd < 0)
			command->valid = 0;
		else
			close(command->infile_fd);
		if (command->here_doc)
			command->here_doc = HERE_DOC_VOID;
	}
}

void	outfile(char **argv, int i, t_command *command)
{
	if (command->valid)
	{
		if (command->outfile_path != NULL)
			free(command->outfile_path);
		else if (ft_strlen(argv[i]) == 1 && argv[i][0] == '>')
			command->outfile_path = ft_strdup(argv[i + 1]);
		command->outfile_fd = open(command->outfile_path,
				O_CREAT | O_WRONLY | O_TRUNC, 00644);
		if (command->outfile_fd < 0)
			command->valid = 0;
		else
			close(command->outfile_fd);
		command->redirection = TRUNC;
	}
}

void	append(char **argv, int i, t_command *command)
{
	if (command->valid)
	{
		if (command->outfile_path != NULL)
			free(command->outfile_path);
		else if (ft_strlen(argv[i]) == 2)
			command->outfile_path = ft_strdup(argv[i + 1]);
		command->outfile_fd = open(command->outfile_path,
				O_CREAT | O_WRONLY | O_APPEND, 00644);
		if (command->outfile_fd < 0)
			command->valid = 0;
		else
			close(command->outfile_fd);
		command->redirection = APPEND;
	}
}
