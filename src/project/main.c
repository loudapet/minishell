/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:15:35 by plouda            #+#    #+#             */
/*   Updated: 2023/08/21 09:16:31 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/*
Use this for valgrind suppression
valgrind -s --leak-check=full --show-reachable=yes --error-limit=no
--suppressions=minishell.supp --gen-suppressions=all ./minishell
*/

int	get_cmd_count(int argc, char **argv)
{
	int	i;
	int count;

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
	hostname_line[ft_strlen(hostname_line) - 1] = '\0'; // newline in prompt fix
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
	//char	*username;
	char	*home_dollar;
	//int		i;

	//i = 0;
	//username = get_env("USER", env);
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

void	printf_list(t_list *lst)
{
	t_list *cur;
	// t_command *con;

	cur = lst;
	while (cur != NULL)
	{
		// con = (t_command *)cur->content;
		// ft_printf("AAA%s\n", con->cmd_args[0]);
		cur = cur->next;
	}
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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*username;
	char	*hostname;
	char	*specs;
	char	*dir;
	char	*prompt;
	char	**env;
	int		i;
	int		status;
	// int		count;
	t_args	args;
	t_command *cmd;
	t_list	*cmds;
	// void	*ptr;
	//t_command pipex;

	(void)argc;
	(void)argv;
	status = 0;

	if (!getenv("USER"))
		return (printf("No...\n"), 0);
	// signal(SIGINT, handler);
	env = create_env(envp);
	username = get_username(env);
	line = "";
	while (1)
	{
		signal(SIGINT, handler);
			signal(SIGQUIT, SIG_IGN);

		i = 0;
		hostname = get_hostname();
		dir = get_directory(env);
		specs = ft_strjoin(username, hostname);
		prompt = ft_strjoin(specs, dir);
		line = readline((const char *)prompt);
		if (line == NULL)
			break ;
		if (line[0] == '\0')
		{
			free(line);
			free(hostname);
			free(specs);
			free(dir);
			free(prompt);
			continue;
		}
		add_history(line);
		args = lexer(line, env, status);
		cmds = NULL;
		while (i < args.ac)
		{
			cmd = command_redirection(args.ac, args.av, &i);
			ft_lstadd_back(&cmds, ft_lstnew(cmd));
		}
		pipex(cmds, &env, &status);
		free_args(args);
		t_list * tmp;
		t_command *tmp_cmd;
		int	k;
		while (cmds != NULL)
		{
			k = 0;
			tmp_cmd = (t_command *)cmds->content;
			while (tmp_cmd->cmd_args[k])
			{
				free(tmp_cmd->cmd_args[k]);
				k++;
			}
			k = 0;
			if (tmp_cmd->delimiter)
			{
				while (tmp_cmd->delimiter[k])
				{
					free(tmp_cmd->delimiter[k]);
					k++;
				}
				free(tmp_cmd->delimiter);
			}
			if (tmp_cmd->outfile_path)
				free(tmp_cmd->outfile_path);
			if (tmp_cmd->infile_path)
				free(tmp_cmd->infile_path);
			free(tmp_cmd->cmd_args);
			free(tmp_cmd);
			tmp = cmds;
			cmds = cmds->next;
			free(tmp);
		}
		if (!ft_strncmp(line, "q", ft_strlen(line)))
			break ;
		free(line);
		free(hostname);
		free(specs);
		free(dir);
		free(prompt);
	}
	free(line);
	free(hostname);
	free(specs);
	free(dir);
	free(prompt);
	free(username);
	free_env(env);
	write(1, "Goodbye!\n", 10);
	return (0);
}
