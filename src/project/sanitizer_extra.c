#include "minishell.h"

int	has_backslash(char *av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (av[i] == '\\')
			return (1);
		i++;
	}
	return (0);
}

int	count_backslashes(char **av, int *i)
{
	int	j;
	int	count;

	*i = 0;
	count = 0;
	while (av[*i])
	{
		j = 0;
		if (!ft_strncmp(av[*i], "\\<<", 4) || !ft_strncmp(av[*i], "\\>", 3)
			|| !ft_strncmp(av[*i], "\\>>", 4) || !ft_strncmp(av[*i], "\\<", 3))
		{
			(*i)++;
			continue ;
		}
		while (av[*i][j])
		{
			if (av[*i][j] == '\\')
				count++;
			j++;
		}
		(*i)++;
	}
	return (count);
}

char	**crimes_against_code(char **av, int i, int j, char **temp)
{
	char	**split_temp;
	int		k;

	while (av[i])
	{
		if (!has_backslash(av[i]))
			temp[j++] = ft_strdup(av[i]);
		else
		{
			k = 0;
			split_temp = ft_split(av[i], '\\');
			while (split_temp[k])
			{
				if (split_temp[k][0] == '<' || split_temp[k][0] == '>')
					temp[j++] = ft_strjoin("\\", split_temp[k]);
				else
					temp[j++] = ft_strdup(split_temp[k]);
				free(split_temp[k++]);
			}
			free(split_temp);
		}
		free(av[i++]);
	}
	temp[j] = NULL;
	return (temp);
}

char	**backslash_splitter(char **av)
{
	int		i;
	int		j;
	char	**temp;

	if (count_backslashes(av, &i) == 0)
		return (av);
	temp = malloc(sizeof(char *) * (i + count_backslashes(av, &i) + 1));
	i = 0;
	j = 0;
	temp = crimes_against_code(av, i, j, temp);
	i = 0;
	free(av);
	return (temp);
}
