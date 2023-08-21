/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:15:59 by plouda            #+#    #+#             */
/*   Updated: 2023/08/21 09:33:36 by plouda           ###   ########.fr       */
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
# include <fcntl.h>
# include <sys/ioctl.h>
# define READ 0
# define WRITE 1
# define TRUNC 1;
# define APPEND 2;
# define HERE_DOC_VOID 1;
# define HERE_DOC_IN 2;

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

typedef struct s_command
{
	char	*infile_path;
	char	*outfile_path;
	int		infile_fd;
	int		outfile_fd;
	int		here_doc;
	int		redirection;
	int		cmd_count;
	char	**cmd_args;
}			t_command;



//Environment variables handling
char	**create_env(char **env);
char	*get_env(char *variable, char **env_arr);
void	free_env(char **env);

// Parsers
t_args	lexer(const char *line, char **env, int status);
void	free_args(t_args args);
char	**minisplit(char const *s, char c);

// Santitizer
char	**sanitizer(int *ac, char **av, char **env, int status);
t_sanitizer	reset_sanitizer(void);
void	quote_counter(const char *c, int *quote, int *single_quote);
char	*expand_env(char *str, char **env, int status);
t_quotes	init_quote_vars(const char *str, int index);
void	copy_front(t_quotes *quotes, int index, const char *str);
void	copy_rest(t_quotes *quotes, const char *str, char type);
char	*expand_and_join(char *str, char *var_name, char *var_value, int index);
void	index_checker(t_sanitizer *san, char **av, int i, int *j);

// Pipex
t_command	*command_redirection(int argc, char **argv, int *i);
void	pipex(t_list *cmds, char ***env, int *status);

// Files
void	here_doc(char **argv, int i, t_command *command);
void	infile(char **argv, int i, t_command *command);
void	outfile(char **argv, int i, t_command *command);
void	append(char **argv, int i, t_command *command);

int	variable_exists(char *arg, char **env);

// Built_ins
void	builtins(char **argv, char ***env, int *status);
int		echo_built(char **argv);
int		cd_built(char **argv, char ***env);
int	cd_home(char ***env);
int		set_new_pwd(char ***env);
int		export_built(char **argv, char ***env);
char	**unset_built(char **arg, char **env);
char	**unset_single(char *arg, char **env);
int		env_built(char **env);
int		pwd_built(char **env);
void	exit_built(char **argv, char **env, int statu);

#endif
