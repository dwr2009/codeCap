#!/bin/sh

RedForground='\e[0;31m'
GreenDenote='\e[0;32m'
NoColorDenote='\e[0m'

git_ignore_file=".gitignore"

if [ ! -e "$git_ignore_file" ]; then
	exit 0
fi

cat "$git_ignore_file"
echo "Do you want remove all git ignore files?[y/n]: n"
read sure
if [ "$sure" != "y" ]; then
	exit 0
fi

while read OneLine || [ -n "${OneLine}" ];
do
	OneLine=${OneLine//$'\r'/};
	OneLine=${OneLine//$'\n'/};
	OneLine=${OneLine//$'\t'/};
	if [ ! -z "${OneLine}" ]; then
		FileToDel="${OneLine}"
		echo -e -n "${RedForground}";
		echo "Delete: ${FileToDel}";
		echo -e -n "${NoColorDenote}";
		rm -fr "${FileToDel}";
		iProcessExitCode=$?;
		if [ ${iProcessExitCode} -ne 0 ]; then
			echo "ERROR: Del(${FileToDel}) failed ${iProcessExitCode}";
		fi;
	fi;
done < "$git_ignore_file";
