/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:15:35 by plouda            #+#    #+#             */
/*   Updated: 2023/07/12 15:56:14 by plouda           ###   ########.fr       */
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

int	main(int argc, char ** argv, char **envp)
{
	char	*line;
	char	*username;
	char	*hostname;
	char	*specs;
	char	*dir;
	char	*prompt;
	char	**env;

	(void)argc;
	(void)argv;

	env = create_env(envp);
	username = get_username(env);
	hostname = get_hostname();
	dir = get_directory(env);
	specs = ft_strjoin(username, hostname);
	prompt = ft_strjoin(specs, dir);
	while (1)
	{
		line = readline((const char *)prompt);
		add_history(line);
		parser(line);
		if (!ft_strncmp(line, "q", ft_strlen(line)))
			break ;
		free(line);
	}
	free(line);
	free(username);
	free(hostname);
	free(specs);
	free(dir);
	free(prompt);
	write(1, "Goodbye!\n", 10);
	return (0);
}