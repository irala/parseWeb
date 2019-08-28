#!/bin/bash
#compile
g++ -g  -o parser -I . main.cpp -Werror -Wall -std=c++17
echo compiled done!