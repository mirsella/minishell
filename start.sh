#!/bin/zsh

# check if $1 is non empty
if [[ -n $1 ]]; then
	echo 'Press Ctrl+C to exit'
	echo 'Press Ctrl+\ to kill minishell'
	trap "exit" SIGINT
	trap "pkill minishell" SIGQUIT
fi

while true; do
	make && ./minishell
done
