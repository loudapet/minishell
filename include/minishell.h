/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:15:59 by plouda            #+#    #+#             */
/*   Updated: 2023/07/21 16:23:17 by plouda           ###   ########.fr       */
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


//Environment variables handling

char	**create_env(char **env);
char	*get_env(char *variable, char **env_arr);
void	free_env(char **env);
void	display_env(char **env);

// Parsers
t_args	lexer(const char *line, char **env);
void	free_args(t_args args);
char	**minisplit(char const *s, char c);

#endif