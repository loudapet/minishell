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

int	is_b(char *com)
{
	if (com == NULL)
		return (0);
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

void	free_files(t_command *tmp_cmd)
{
	if (tmp_cmd->outfile_path)
		free(tmp_cmd->outfile_path);
	if (tmp_cmd->infile_path)
		free(tmp_cmd->infile_path);
	if (tmp_cmd->args != NULL)
		free(tmp_cmd->args);
	free(tmp_cmd);
}

void	free_cmd(t_freebs stuff)
{
	t_list		*tmp;
	t_command	*tmp_cmd;
	int			k;

	while (*(stuff.cmds) != NULL)
	{
		k = 0;
		tmp_cmd = (t_command *)(*stuff.cmds)->content;
		while (tmp_cmd->args != NULL && tmp_cmd->args[k])
			free(tmp_cmd->args[k++]);
		k = 0;
		if (tmp_cmd->delimiter)
		{
			while (tmp_cmd->delimiter[k])
				free(tmp_cmd->delimiter[k++]);
			free(tmp_cmd->delimiter);
		}
		free_files(tmp_cmd);
		tmp = (*stuff.cmds);
		*(stuff.cmds) = (*stuff.cmds)->next;
		free(tmp);
	}
}

void	free_stuff(t_freebs stuff, int l)
{
	int	i;

	i = 0;
	free(*(stuff.line));
	free(*(stuff.hostname));
	free(*(stuff.specs));
	free(*(stuff.dir));
	free(*(stuff.prompt));
	free(*(stuff.username));
	free_cmd(stuff);
	while ((*(stuff.env))[i])
		free(((*(stuff.env))[i++]));
	free(*(stuff.env));
	i = 0;
	if (stuff.fd_n != 0)
	{
		while (i <= l)
			free((*stuff.fd)[i++]);
		free(*stuff.fd);
	}
	i = 0;
	while ((*stuff.args)[i])
		free((*stuff.args)[i++]);
	free(*stuff.args);
}

void	here_doc_check(t_list *cmds)
{
	t_command	*cmd;

	cmd = (t_command *)cmds->content;
	if (cmd->here_doc == HERE_DOC_IN && !cmd->valid)
		cmd->here_doc = HERE_DOC_VOID;
}
