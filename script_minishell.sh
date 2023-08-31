#!/usr/bin/bash

if [[ $1 == "--valgrind" ]];
then
	valgrind -s --leak-check=full --show-reachable=yes --error-limit=no --suppressions=minishell.supp ./minishell;
else
	./minishell;
fi