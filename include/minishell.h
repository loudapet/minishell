/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:15:59 by plouda            #+#    #+#             */
/*   Updated: 2023/08/31 15:10:18 by plouda           ###   ########.fr       */
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
# include <limits.h>
# include <errno.h>
# define READ 0
# define WRITE 1
# define TRUNC 1
# define APPEND 2
# define HERE_DOC_VOID 1
# define HERE_DOC_IN 2

extern int	g_signal;

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
	int		here_doc_counter;
	char	**delimiter;
	int		redirection;
	int		cmd_count;
	char	**args;
	int		valid;
	int		heredoc_pipe[2];
	pid_t	pid;
}			t_command;

typedef struct s_freebs
{
	char	**line;
	char	**hostname;
	char	**specs;
	char	**dir;
	char	**prompt;
	char	**username;
	char	***env;
	t_list	**cmds;
	int		***fd;
	int		fd_n;
	char	***args;
	int		*status;
}			t_freebs;

typedef struct s_pipe_variables
{
	int		i;
	int		l;
	int		out;
	int		in;
	int		stat;
}				t_pipe_variables;

typedef struct 	s_prompt_variables
{
	char	*line;
	char	*username;
	char	*hostname;
	char	*specs;
	char	*dir;
	char	*prompt;
	t_args	args;
	int		status;
	int		i;
}				t_prompt_variables;

//Environment variables handling
char		**create_env(char **env);
char		*get_env(char *variable, char **env_arr);
void		free_env(char **env);

// Parsers
t_args		lexer(const char *line, char **env, int status);
void		free_args(t_args args);
char		**minisplit(char const *s, char c);
char		**slice_redirections(char **av, int *ac);

// Santitizer
t_sanitizer	reset_sanitizer(void);
void		quote_counter(const char *c, int *quote, int *single_quote);
char		*expand_env(char *str, char **env, int status);
t_quotes	init_quote_vars(const char *str, int index);
void		copy_front(t_quotes *quotes, int index, const char *str);
void		copy_rest(t_quotes *quotes, const char *str, char type);
char		*expand_and_join(char *str, char *var_name,
				char *var_value, int index);
void		sanitize(t_sanitizer *san, char **av, int i, int *j);

// Pipex
t_command	*parser(int argc, char **argv, int *i);
int			pipex(t_list *cmds, char ***env, int *status, t_freebs stuff);
void		heredoc_handler(t_list *cmds, t_freebs stuff);
int			heredoc_exec(t_command *command, int flag);
void		waiter(t_list *cmds, int *status);
void		handler2(int sig);

// Freeing and Checks
void		here_doc_check(t_list *cmds);
int			is_b(char *com);
void		free_stuff(t_freebs stuff, int l);
void		free_cmd(t_freebs stuff);
void		free_files(t_command *tmp_cmd);

// Forking and Things
void		forked_processes(t_pipe_variables var,
				t_freebs stuff, int **fd, t_list *cmds);
void		pipe_waiter(t_pipe_variables var, int *status, int **fd);
void		pipe_parent(t_list *cmds, t_pipe_variables var, int **fd);
void		pipe_child(t_pipe_variables var,
				t_freebs st, int **fd, t_list *cmds);
void		no_pipe(t_list *cmds, t_pipe_variables var,
				char ***env, int *status);

// Duping
void		dup_infile(t_command *cmd,
				t_pipe_variables var, t_freebs st, int **fd);
void		dup_outfile(t_command *cmd,
				t_pipe_variables var, t_freebs st, int **fd);

// Files
void		here_doc(char **argv, int i, t_command *command);
void		str_stuff(char **str, int mode, char *delimiter);
void		fake_heredoc(t_command *command, char *str,
				char **delimiter, int *i);
void		infile(char **argv, int i, t_command *command);
void		outfile(char **argv, int i, t_command *command);
void		append(char **argv, int i, t_command *command);

int			variable_exists(char *arg, char **env);

// Built_ins
void		builtins(char **argv, char ***env, int *status);
int			echo_built(char **argv);
int			cd_built(char **argv, char ***env);
int			cd_home(char ***env);
int			set_new_pwd(char ***env);
int			export_built(char **argv, char ***env);
void		show_var(char ***env);
char		**set_new_env(char *arg, char **env);
char		**unset_built(char **arg, char **env);
char		**unset_single(char *arg, char **env);
int			env_built(char **env);
int			pwd_built(char **env);
void		exit_built(char **argv, char **env, int status);
void		display_argv(char **argv);

#endif
