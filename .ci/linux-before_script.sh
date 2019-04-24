#!/usr/bin/env bash

# # Fake display
# export DISPLAY=:99.0
# sh -e /etc/init.d/xvfb start
# sleep 3 # give xvfb some time to start
# sudo apt-get install libsndfile1

# Set PROJUCER to GPL mode
cd $TRAVIS_BUILD_DIR/3rd_party/JUCE
cd extras/Projucer/JuceLibraryCode
pwd
sed -i "s/#define JUCER_ENABLE_GPL_MODE 0/#define JUCER_ENABLE_GPL_MODE 1/" AppConfig.h
sed -i "s/#define JUCE_USE_DARK_SPLASH_SCREEN 1/#define JUCE_USE_DARK_SPLASH_SCREEN 0/" AppConfig.h
sed -i "s/#define JUCE_WEB_BROWSER 1/#define JUCE_WEB_BROWSER 0/" AppConfig.h
cat AppConfig.h

# Build PROJUCER
cd ../Builds/LinuxMakefile
make -j4 CONFIG=Release
cd $TRAVIS_BUILD_DIR