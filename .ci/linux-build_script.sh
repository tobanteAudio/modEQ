#!/usr/bin/env bash

ls
cd $TRAVIS_BUILD_DIR

# Set gcc version
sudo ln -s /usr/bin/gcc-6 /usr/local/bin/gcc
sudo ln -s /usr/bin/g++-6 /usr/local/bin/g++
gcc -v && g++ -v

# Generate Makefile
$TRAVIS_BUILD_DIR/3rd_party/JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --resave $TRAVIS_BUILD_DIR/modEQ.jucer
  
# Build
cd Builds/LinuxMakefile
make -j4 CONFIG=Release