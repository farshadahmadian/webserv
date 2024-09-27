#!/bin/bash

G="\033[0;32m";
Y="\033[0;33m";
R="\033[0;31m";
NC="\033[0m";

# Make with VERBOSE parameter to 0
make fclean;
make;

# Valid maps tester
CONF_FOLDER=./conf/valid/;
CONF_FILES=$(ls "$CONF_FOLDER");
echo -e "\n[ VALID CONF ]";
for FILE in $CONF_FILES; do
	./webserv "$CONF_FOLDER$FILE";
	if [[ $? == 0 ]]; then
		echo -e "${G}[OK]   ${NC} [$FILE]";
	else
		echo -e "${R}[FAIL] ${NC} [$FILE]"
	fi;
done;

echo -e "\n";

# Invalid maps tester
CONF_FOLDER=./conf/invalid/;
CONF_FILES=$(ls "$CONF_FOLDER");
echo "[ INVALID CONF ]";
for FILE in $CONF_FILES; do
	./webserv "$CONF_FOLDER$FILE";
	if [[ $? == 0 ]]; then
		echo -e "${Y}[FAIL] ${NC} [$FILE]\n"
	else
		echo -e "${G}[OK]   ${NC} [$FILE]\n";
	fi;
done;
