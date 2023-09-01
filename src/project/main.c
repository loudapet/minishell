/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:15:35 by plouda            #+#    #+#             */
/*   Updated: 2023/08/31 15:39:05 by plouda           ###   ########.fr       */
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
void	free_args(t_args args)
{
	int	i;

	i = 0;
	while (args.av[i])
	{
		free(args.av[i]);
		i++;
	}
	free(args.av);
}

int	get_cmd_count(int argc, char **argv)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (i < argc)
	{
		if (!strncmp(argv[i], "|", 1))
			count++;
		i++;
	}
	return (count);
}

size_t	get_hostname_len(char *hostname)
{
	size_t	i;

	i = 0;
	while (hostname[i] != '.' && hostname[i] != '\0')
		i++;
	return (i);
}

char	*get_hostname(void)
{
	int		fd;
	char	*hostname_line;
	char	*hostname_raw;
	char	*hostname;

	fd = open("/etc/hostname", O_RDONLY);
	hostname_line = get_next_line(fd);
	close(fd);
	hostname_line[ft_strlen(hostname_line) - 1] = '\0';
	hostname_raw = ft_substr(hostname_line, 0, get_hostname_len(hostname_line));
	hostname = ft_strjoin(hostname_raw, ":");
	free(hostname_line);
	free(hostname_raw);
	return (hostname);
}

char	*get_username(char **env)
{
	char	*username;

	username = get_env("USER", env);
	username = ft_strjoin(username, "@");
	return (username);
}

// needs more testing for dirs above home
char	*get_directory(char **env)
{
	char	*abs_path;
	char	*home;
	char	*env_home;
	char	*home_dollar;

	abs_path = getcwd(NULL, 0);
	env_home = get_env("HOME", env);
	if (env_home != NULL && !strncmp(abs_path, env_home, ft_strlen(env_home)))
		home = abs_path + ft_strlen(env_home);
	else
		home = NULL;
	if (home == NULL)
	{
		home_dollar = ft_strjoin(abs_path, "$ ");
		free(abs_path);
		return (home_dollar);
	}
	home = ft_strjoin("~", home);
	home_dollar = ft_strjoin(home, "$ ");
	free(abs_path);
	free(home);
	return (home_dollar);
}

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

int	is_thingy(char c, int *pipe)
{
	if (c == '>' || c == '<' || c == '|')
	{
		if (c == '|')
			*pipe = 1;
		else if (*pipe == 1)
		{
			*pipe = 0;
			return (0);
		}
		return (1);
	}
	return (0);
}

void	move_index(char *line, int *i)
{
	if (line[*i] == '>' && line[(*i) + 1] == '>')
		(*i)++;
	else if (line[*i] == '<' && line[(*i) + 1] == '<')
		(*i)++;
}

int	space_mover_pipe_checker(char *line, int *i, int *quote, int *pipe)
{
	*i = 0;
	*quote = 0;
	*pipe = 0;
	while (line[*i] == ' ')
		i++;
	if (line[*i] == '|')
	{
		ft_putstr_fd("Invalid syntax\n", 2);
		return (0);
	}
	return (1);
}

int	error_returner(void)
{
	ft_putstr_fd("Invalid syntax\n", 2);
	return (0);
}

void	quote_state(char *line, int *quote, int i)
{
	if (line[i] == '\'' && *quote == 0)
		*quote = 1;
	else if (line[i] == '"' && *quote == 0)
		*quote = 2;
	else if (line[i] == '\'' && *quote == 1)
		*quote = 0;
	else if (line[i] == '"' && *quote == 2)
		*quote = 0;
}

int	syn_ch(char *line)
{
	int	i;
	int	quote;
	int	pipe;

	if (!space_mover_pipe_checker(line, &i, &quote, &pipe))
		return (0);
	while (line[i])
	{
		quote_state(line, &quote, i);
		if (quote == 0 && is_thingy(line[i], &pipe))
		{
			move_index(line, &i);
			i++;
			while (line[i] == ' ')
				i++;
			if (is_thingy(line[i], &pipe) || line[i] == '\0')
				return (error_returner());
		}
		i++;
	}
	return (1);
}

int	quot_ch(char *line)
{
	int	i;
	int	quote;

	quote = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' && quote == 0)
			quote = 1;
		else if (line[i] == '"' && quote == 0)
			quote = 2;
		else if (line[i] == '\'' && quote == 1)
			quote = 0;
		else if (line[i] == '"' && quote == 2)
			quote = 0;
		i++;
	}
	if (quote != 0)
	{
		ft_putstr_fd("Close your quotes! >:C\n", 2);
		return (0);
	}
	return (1);
}

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

void	innit_loop(t_prompt_variables *pr_var, char **env)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	pr_var->i = 0;
	pr_var->hostname = get_hostname();
	pr_var->dir = get_directory(env);
	pr_var->specs = ft_strjoin(pr_var->username, pr_var->hostname);
	pr_var->prompt = ft_strjoin(pr_var->specs, pr_var->dir);
	pr_var->line = readline((const char *)pr_var->prompt);
}

void	innit_start_values(t_prompt_variables *pr_var, char **env, int *status)
{
	g_signal = 0;
	pr_var->username = get_username(env);
	pr_var->line = "";
	*status = 0;
}

void	minishell_runner_2(t_freebs stuff,
	t_prompt_variables pr_var, char **env, t_list *cmds)
{
	adding_freeables(&stuff, &pr_var, &env, &cmds);
	heredoc_handler(cmds, stuff);
	if (g_signal == 0)
		pipex(cmds, &env, &pr_var.status, stuff);
	free_after_commands(&pr_var, cmds);
}

int	check_for_empty(t_prompt_variables pr_var)
{
	if (pr_var.line == NULL)
		return (0);
	if (pr_var.line[0] != '\0')
		add_history(pr_var.line);
	return (1);
}

int	minishell_runner(char **env, t_prompt_variables pr_var)
{
	t_freebs			stuff;
	t_command			*cmd;
	t_list				*cmds;

	while (1)
	{
		innit_loop(&pr_var, env);
		if (!check_for_empty(pr_var))
			break ;
		if (!pr_var.line[0] || !syn_ch(pr_var.line) || !quot_ch(pr_var.line))
		{
			free_exit_or_empty(&pr_var, env, 1);
			continue ;
		}
		pr_var.args = lexer(pr_var.line, env, pr_var.status);
		cmds = NULL;
		while (pr_var.i < pr_var.args.ac)
		{
			cmd = parser(pr_var.args.ac, pr_var.args.av, &pr_var.i);
			ft_lstadd_back(&cmds, ft_lstnew(cmd));
		}
		minishell_runner_2(stuff, pr_var, env, cmds);
	}
	return (free_exit_or_empty(&pr_var, env, 0), pr_var.status);
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
	minishell_runner(env, pr_var);
	return (pr_var.status);
}
