/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehasalu <ehasalu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:15:35 by plouda            #+#    #+#             */
/*   Updated: 2023/09/01 15:54:02 by ehasalu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
