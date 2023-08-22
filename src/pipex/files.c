/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 11:44:04 by plouda            #+#    #+#             */
/*   Updated: 2023/08/22 10:49:36 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Should probably not pass a whole stucture if it can be avoided

void	here_doc(char **argv, int i, t_command *command)
{
	if (command->infile_path != NULL)
		free(command->infile_path);
	command->infile_path = NULL;
	/* if (ft_strlen(argv[i]) > 2)
		command->infile_path = ft_strdup(&argv[i][1]);
	else if (ft_strlen(argv[i]) == 2)
		command->infile_path = ft_strdup(argv[i + 1]); */
	command->here_doc = HERE_DOC_IN;
	command->delimiter = ft_strdup(argv[i + 1]);
}

void	infile(char **argv, int i, t_command *command)
{
	if (command->infile_path != NULL)
		free(command->infile_path);
	if (ft_strlen(argv[i]) > 1 && argv[i][0] == '<')
		command->infile_path = ft_strdup(&argv[i][1]);
	else if (ft_strlen(argv[i]) == 1 && argv[i][0] == '<')
		command->infile_path = ft_strdup(argv[i + 1]);
	command->infile_fd = open(command->infile_path, O_RDONLY); // what mode do infiles open in, and do I even need to open them here?
	close(command->infile_fd);
	if (command->here_doc)
		command->here_doc = HERE_DOC_VOID;
}

void	outfile(char **argv, int i, t_command *command)
{
	if (command->outfile_path != NULL)
		free(command->outfile_path);
	if (ft_strlen(argv[i]) > 1 && argv[i][0] == '>')
		command->outfile_path = ft_strdup(&argv[i][1]);
	else if (ft_strlen(argv[i]) == 1 && argv[i][0] == '>')
		command->outfile_path = ft_strdup(argv[i + 1]);
	command->outfile_fd = open(command->outfile_path, O_CREAT | O_WRONLY | O_TRUNC, 00644) ;
	close(command->outfile_fd);
	command->redirection = TRUNC;
}

void	append(char **argv, int i, t_command *command)
{
	if (command->outfile_path != NULL)
		free(command->outfile_path);
	if (ft_strlen(argv[i]) > 2)
		command->outfile_path = ft_strdup(&argv[i][1]);
	else if (ft_strlen(argv[i]) == 2)
		command->outfile_path = ft_strdup(argv[i + 1]);
	command->outfile_fd = open(command->outfile_path, O_CREAT | O_WRONLY | O_APPEND, 00644);
	close(command->outfile_fd);
	command->redirection = APPEND;
}
