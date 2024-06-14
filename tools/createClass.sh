#!/bin/bash

c++ -Wall -Wextra -Werror ./createClass.cpp -o createClass;
./createClass $@;
rm -rf createClass;
