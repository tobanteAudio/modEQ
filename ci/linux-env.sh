#!/usr/bin/env bash

# Sets the compiler   
sudo ln -s /usr/bin/gcc-6 /usr/local/bin/gcc
sudo ln -s /usr/bin/g++-6 /usr/local/bin/g++
gcc -v && g++ -v