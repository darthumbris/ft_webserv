#/bin/bash

if [[ $(uname) = "Darwin" ]];
then
	printf "\e[42m$(uname)\e[0m\n"
	#set up needed folders
	mkdir -p ~/.vim/autoload ~/.vim/bundle
	#check if pathagoen is instaled
	if [ ! -f ~/.vim/autoload/pathogen.vim ]
	then
		#download pathogen vim
		printf "\e[94mINSTALLING PATHOGEN\e[0m\n"
		curl -kLSso ~/.vim/autoload/pathogen.vim https://tpo.pe/pathogen.vim
	else
		printf "\e[32mPATGOEN IS INSTALLED\e[0m\n"
	fi
fi
