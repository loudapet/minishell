/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehasalu <ehasalu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:15:35 by plouda            #+#    #+#             */
/*   Updated: 2023/09/01 15:52:33 by ehasalu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

/*
Use this for valgrind suppression
valgrind -s --leak-check=full --show-reachable=yes --error-limit=no
--suppressions=minishell.supp --gen-suppressions=all ./minishell
*/

// maybe consider redoing this for argv[i] != NULL

void	innit_start_values(t_prompt_variables *pr_var, char **env, int *status)
{
	g_signal = 0;
	pr_var->username = get_username(env);
	pr_var->line = "";
	*status = 0;
}

void	minishell_runner_2(t_prompt_variables *pr_var,
		char ***env, t_list *cmds)
{
	t_freebs			stuff;

	adding_freeables(&stuff, pr_var, env, &cmds);
	heredoc_handler(cmds, stuff);
	if (g_signal == 0)
		pipex(cmds, env, &pr_var->status, stuff);
	free_after_commands(pr_var, cmds);
}

int	check_for_empty(t_prompt_variables pr_var)
{
	if (pr_var.line == NULL)
		return (0);
	if (pr_var.line[0] != '\0')
		add_history(pr_var.line);
	return (1);
}

int	minishell_runner(char ***env, t_prompt_variables pr_var)
{
	t_command			*cmd;
	t_list				*cmds;

	while (1)
	{
		innit_loop(&pr_var, *env);
		if (!check_for_empty(pr_var))
			break ;
		if (!pr_var.line[0] || !only_space(pr_var.line)
			|| !syn_ch(pr_var.line) || !quot_ch(pr_var.line))
		{
			free_exit_or_empty(&pr_var, *env, 1);
			continue ;
		}
		pr_var.args = lexer(pr_var.line, *env, pr_var.status);
		cmds = NULL;
		while (pr_var.i < pr_var.args.ac)
		{
			cmd = parser(pr_var.args.ac, pr_var.args.av, &pr_var.i);
			ft_lstadd_back(&cmds, ft_lstnew(cmd));
		}
		minishell_runner_2(&pr_var, env, cmds);
	}
	return (free_exit_or_empty(&pr_var, *env, 0), pr_var.status);
}

int	main(int argc, char **argv, char **envp)
{
	char				**env;
	t_prompt_variables	pr_var;

	(void)argc;
	(void)argv;
	if (!getenv("USER"))
		return (printf("No...\n"), 0);
	env = create_env(envp);
	innit_start_values(&pr_var, env, &pr_var.status);
	minishell_runner(&env, pr_var);
	return (pr_var.status);
}
