#include "minishell.h"

char	**create_env(char **env)
{
	int		i;
	char	**env_arr;

	i = 0;
	while (env[i])
		i++;
	env_arr = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		env_arr[i] = ft_strdup(env[i]);
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

char	*get_env(char *variable, char **env_arr)
{
	int	i;
	int	l;

	i = 0;
	while (env_arr[i])
	{
		l = 0;
		while (env_arr[i][l] != '=')
			l++;
		if (!ft_strncmp(variable, env_arr[i], l) && l == ft_strlen(variable))
			break;
		i++;
	}
	if (!env_arr[i])
		return (NULL);
	while (*(env_arr[i]) != '=')
		(env_arr[i])++;
	(env_arr[i])++;
	if (*(env_arr[i]))
		return (env_arr[i]);
	else
		return (NULL);
}