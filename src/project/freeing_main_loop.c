/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing_main_loop.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehasalu <ehasalu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 15:45:14 by ehasalu           #+#    #+#             */
/*   Updated: 2023/09/01 15:53:59 by ehasalu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_files_and_args(t_command *tmp_cmd)
{
	if (tmp_cmd->outfile_path)
		free(tmp_cmd->outfile_path);
	if (tmp_cmd->infile_path)
		free(tmp_cmd->infile_path);
	if (tmp_cmd->args != NULL)
		free(tmp_cmd->args);
}

void	free_list(t_list *cmds)
{
	t_list		*tmp;
	t_command	*tmp_cmd;
	int			k;

	while (cmds != NULL)
	{
		k = 0;
		tmp_cmd = (t_command *)cmds->content;
		while (tmp_cmd->args != NULL && tmp_cmd->args[k])
			free(tmp_cmd->args[k++]);
		k = 0;
		if (tmp_cmd->delimiter)
		{
			while (tmp_cmd->delimiter[k])
				free(tmp_cmd->delimiter[k++]);
			free(tmp_cmd->delimiter);
		}
		free_files_and_args(tmp_cmd);
		free(tmp_cmd);
		tmp = cmds;
		cmds = cmds->next;
		free(tmp);
	}
}

void	free_exit_or_empty(t_prompt_variables *pr_var, char **env, int flag)
{
	free(pr_var->line);
	free(pr_var->hostname);
	free(pr_var->specs);
	free(pr_var->dir);
	free(pr_var->prompt);
	if (flag == 0)
	{
		free(pr_var->username);
		free_env(env);
		write(1, "Goodbye!\n", 10);
	}
}

void	free_after_commands(t_prompt_variables *pr_var, t_list *cmds)
{
	free_args(pr_var->args);
	free_list(cmds);
	free(pr_var->line);
	free(pr_var->hostname);
	free(pr_var->specs);
	free(pr_var->dir);
	free(pr_var->prompt);
}

void	adding_freeables(t_freebs *stuff, t_prompt_variables *pr_var,
		char ***env, t_list **cmds)
{
	stuff->line = &pr_var->line;
	stuff->hostname = &pr_var->hostname;
	stuff->specs = &pr_var->specs;
	stuff->dir = &pr_var->dir;
	stuff->prompt = &pr_var->prompt;
	stuff->username = &pr_var->username;
	stuff->env = env;
	stuff->cmds = cmds;
	stuff->args = &pr_var->args.av;
	stuff->fd_n = 0;
}
