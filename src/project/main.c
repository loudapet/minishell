/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:15:35 by plouda            #+#    #+#             */
/*   Updated: 2023/07/31 16:23:03 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/*
Use this for valgrind suppression
valgrind -s --leak-check=full --show-reachable=yes --error-limit=no
--suppressions=minishell.supp --gen-suppressions=all ./minishell
*/

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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*username;
	char	*hostname;
	char	*specs;
	char	*dir;
	char	*prompt;
	char	**env;
	t_args	args;

	(void)argc;
	(void)argv;

	if (!getenv("USER"))
		return (printf("No...\n"), 0);
	env = create_env(envp);
	username = get_username(env);
	while (1)
	{
		hostname = get_hostname();
		dir = get_directory(env);
		specs = ft_strjoin(username, hostname);
		prompt = ft_strjoin(specs, dir);
		line = readline((const char *)prompt);
		add_history(line);
		args = lexer(line, env);
		copy_argv(args.ac, args.av);
		free_args(args);
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