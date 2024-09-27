#!/bin/bash
printf "<html><body><center><h1>Hello from Bash!</h1></center></body></html>"

echo  $'\n----- THE SCRIPT COMMAND -----\n'
sleep 2;
ls -la;
# echo $'\n------------ ENV VARIABLS -------------\n'
# printf "Content-type: text/plain\n\n";
# printf "INFO_PATH: $PATH_INFO \n";
# printf "SCRIPT_NAME: $SCRIPT_NAME \n";
# printf "REQUEST_METHOD: $REQUEST_METHOD \n";
# printf "SERVER_NAME: $SERVER_NAME \n";
# printf "SERVER_PROTOCOL: $SERVER_PROTOCOL \n";
# printf "SERVER_PORT: $SERVER_PORT \n";
# printf "HOST_NAME: $HOST_NAME \n";
# echo $'\n----- THE SCRIPT ENDS ------'
