# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehasalu <ehasalu@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/03 15:05:09 by plouda            #+#    #+#              #
#    Updated: 2023/09/01 17:02:52 by ehasalu          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRCS =	project/main.c \
		project/main2.c \
		project/main_init.c \
		project/main_checkers.c \
		project/freeing_main_loop.c \
		project/space_check.c \
		lexer/env_variables.c \
		lexer/lexer.c \
		lexer/minisplit.c \
		lexer/expand_vars.c \
		lexer/slicer.c \
		lexer/slicer2.c \
		lexer/slicer3.c \
		parser/quote_remover.c \
		parser/quote_remover_helpers.c \
		parser/parser.c \
		parser/parser2.c \
		parser/files.c \
		pipex/pipex.c \
		pipex/duping.c \
		pipex/forking_and_things.c \
		pipex/freeing_and_checks.c \
		pipex/heredoc.c \
		pipex/heredoc2.c \
		builtins/echo.c \
		builtins/builtins.c \
		builtins/cd.c \
		builtins/cd_extra.c \
		builtins/export.c \
		builtins/export2.c \
		builtins/unset.c \
		builtins/env.c \
		builtins/pwd.c \
		builtins/exit.c

BUILD_DIR = ./obj
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS))
OBJS := $(OBJS:%.c=%.o)
SRCS := $(addprefix $(SRCS_DIR)/, $(SRCS))

LIBFTPRINTF = libftprintf
AR = ar rc
CC = cc -g -Wall -Wextra -Werror
FLAGS = -lreadline

HEADERS = -I ./include/

# Colors (credits to xhelp00)
GREEN		=	$(shell printf "\033[0;32m")
YELLOW		=	$(shell printf "\033[0;33m")
RESET		=	$(shell printf "\033[0m")

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Compiling libft...$(RESET)"
	@make --no-print-directory -C $(LIBFTPRINTF)
	@mv libftprintf/libftprintf.a .
	@echo "$(YELLOW)Linking objects...$(RESET)"
	@$(CC) $(OBJS) $(LIBFTPRINTF).a -o $(NAME) $(FLAGS)
	@echo "$(GREEN)Compilation successful.$(RESET)"

obj/%.o: src/%.c | objdir
	@$(CC) -c $^ $(HEADERS) -o $@

objdir:
	@mkdir -p obj
	@mkdir -p obj/project
	@mkdir -p obj/lexer
	@mkdir -p obj/parser
	@mkdir -p obj/pipex
	@mkdir -p obj/builtins


clean:
	@echo "$(YELLOW)Removing object files...$(RESET)"
	@rm -rf ./obj
	@make clean --no-print-directory -C $(LIBFTPRINTF)

fclean: clean
	@echo "$(YELLOW)Removing executable...$(RESET)"
	@rm -f $(NAME)
	@rm -f libftprintf.a
	@rm -f libftprintf/libft/libft.a

re: fclean all

.PHONY: all, clean, fclean, re, bonus

