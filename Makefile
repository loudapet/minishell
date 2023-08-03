# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: plouda <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/03 15:05:09 by plouda            #+#    #+#              #
#    Updated: 2023/08/03 10:19:48 by plouda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRCS =	project/main.c \
		project/env_variables.c \
		project/lexer.c \
		project/minisplit.c \
		project/sanitizer.c \
		project/sanitizer_helpers.c \
		project/expand_vars.c \
		pipex/command_redirection.c \
		pipex/files.c

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
	@mkdir -p obj/pipex

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

