#!/bin/bash

G="\033[0;32m";
Y="\033[0;33m";
R="\033[0;31m";
NC="\033[0m";
SILENT="2>/dev/null 1>/dev/null"

# Valid maps tester
CONF_FOLDER=./conf/valid/;
CONF_FILES=$(ls "$CONF_FOLDER");
echo -e "\n[ VALID CONF ]";
for FILE in $CONF_FILES; do
	echo -n "[$FILE]"
	./webserv "$CONF_FOLDER$FILE" "$SILENT";
	if [[ $? == 0 ]]; then
		echo -e "${G}[OK]${NC}";
	else
		echo -e "${R}[FAIL]${NC}"
	fi;
done;

echo -e "\n";

# Invalid maps tester
CONF_FOLDER=./conf/invalid/;
CONF_FILES=$(ls "$CONF_FOLDER");
echo "[ INVALID CONF ]";
for FILE in $CONF_FILES; do
	echo "[$FILE]"
	./webserv "$CONF_FOLDER$FILE" "$SILENT";
	if [[ $? == 0 ]]; then
		echo -e "${R}[FAIL]${NC}\n"
	else
		echo -e "${G}[OK]${NC}\n";
	fi;
done;
