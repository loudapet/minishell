/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:15:59 by plouda            #+#    #+#             */
/*   Updated: 2023/07/31 16:23:13 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libftprintf/ft_printf.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/wait.h>
# define READ 0
# define WRITE 1

typedef struct s_args
{
	char	**av;
	int		ac;
}			t_args;

typedef struct s_sanitizer
{
	int	index;
	int	quote;
	int	single_quote;
}				t_sanitizer;

typedef struct s_quotes
{
	char	*str_san;
	int		quote_counter;
	int		i;
	int		j;
	int		k;
}				t_quotes;

typedef struct s_minisplit
{
	int		i;
	int		j;
	int		flag;
	int		count;
	int		quote;
	int		single_quote;
	int		pos;
}				t_minisplit;

typedef struct s_files
{
	char	*infile_path;
	char	*outfile_path;
	int		infile_fd;
	int		outfile_fd;
}			t_files;



//Environment variables handling
char	**create_env(char **env);
char	*get_env(char *variable, char **env_arr);
void	free_env(char **env);

// Parsers
t_args	lexer(const char *line, char **env);
void	free_args(t_args args);
char	**minisplit(char const *s, char c);

// Santitizer
char	**sanitizer(int ac, char **av, char **env);
t_sanitizer	reset_sanitizer(void);
void	quote_counter(const char *c, int *quote, int *single_quote);
char	*expand_env(char *str, char **env);
t_quotes	init_quote_vars(const char *str, int index);
void	copy_front(t_quotes *quotes, int index, const char *str);
void	copy_rest(t_quotes *quotes, const char *str, char type);
char	*expand_and_join(char *str, char *var_name, char *var_value, int index);

// Pipex
char	**copy_argv(int argc, char **argv);

#endif