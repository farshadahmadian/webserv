#!/bin/bash

c++ -Wall -Wextra -Werror ./tools/createClass.cpp -o ./tools/createClass;
./tools/createClass $@;
rm -rf ./tools/createClass;
