/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 11:37:49 by plouda            #+#    #+#             */
/*   Updated: 2023/08/01 10:36:07 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// doesn't work for >outfile and <infile yet
void	get_files(int argc, char **argv, t_files *files)
{
	int	i;

	i = 0;
	while (i < argc)
	{
		if (argv[i][0] == '<')
			files->infile_path = argv[i + 1];
		else if (argv[i][0] == '>')
			files->outfile_path = argv[i + 1]; // also needs to get truncated
		i++;
	}
}

int	get_cmd_args_len(int argc, char **argv)
{
	int	i;
	int	len;

	i = 0;
	len = argc;
	while (i < argc)
	{
		if (argv[i][0] == '<' || argv[i][0] == '>' || argv[i][0] == '|')
			len -= 2;
		i++;
	}
	return (len);
}

char	**get_cmd_args(int argc, char **argv, char **argv_cpy)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < argc)
	{
		while (argv[i][0] == '<' || argv[i][0] == '>' || argv[i][0] == '|')
		{	
			i += 2;
			if (i >= argc)
				break ;
		}
		if (i >= argc)
			break ;
		argv_cpy[j] = argv[i];
		i++;
		j++;
	}
	argv_cpy[j] = NULL;
	return (argv_cpy);
}

char	**copy_argv(int argc, char **argv)
{
	int		i;
	int		j;
	int		cmd_args_len;
	char	**argv_cpy;
	t_files	files;

	cmd_args_len = get_cmd_args_len(argc, argv);
	argv_cpy = malloc(sizeof(char *) * (cmd_args_len + 1));
	i = 0;
	while (i < argc)
	{
		get_files(argc, argv, &files);
		argv_cpy = get_cmd_args(argc, argv, argv_cpy);
		i++;
	}
	ft_printf("Infile: %s\nOutfile: %s\n", files.infile_path, files.outfile_path);
	i = 0;
	while (argv_cpy[i])
	{
		j = 0;
		while (argv_cpy[i][j])
		{
			write(1, &argv_cpy[i][j], 1);
			j++;
		}
		write(1, " ", 1);
		i++;
	}
	write(1, "\n", 1);
	return (argv_cpy);
}

/* char	**copy_argv(int argc, char **argv)
{
	int	i;
	char	**argv_cpy;

	i = 0;
	argv_cpy = malloc(sizeof(char *) * (argc + 1));
	while (i < argc)
	{
		argv_cpy[i] = ft_strdup(argv[i]);
		ft_printf("%s\n", argv_cpy[i]);
		if (!argv_cpy[i][0])
			ft_printf("I'm NULL\n");
		i++;
	}
	argv[i] = NULL;
	return (argv_cpy);
} */

/* t_files	resolve_redirections(char **argv, int start, int end)
{
	t_files	pipex;
	
	while (start <= end)
	{
		if (!ft_strncmp(argv[start], "<", ft_strlen(argv[start])))
		{
			pipex.infile_path = argv[start + 1];
		}
		if (!ft_strncmp(argv[start], ">", ft_strlen(argv[start])))
		{
			pipex.outfile_path = argv[start + 1];
		}
		start++;
	}
}

void	pipex(int argc, char **argv, char **envp)
{
	int		start;
	int		pipe;
	char	**argv_cpy;

	start = 0;
	pipe = 0;
	while (pipe <= argc 
		&& ft_strncmp(argv[pipe], "|", ft_strlen(argv[pipe])))
	{
		resolve_redirections();
		pipe++;
	}
} */
