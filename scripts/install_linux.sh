#!/bin/sh

JUCE_PROJECT_NAME=modEQ
DEFAULT_BUILD_MODE=Release
BUILD_MODE=$1
VST_DIRECTORY=~/.vst/tobanteAudio

# HELP SECTION
if [ "$1" == "-h" ]; then
    echo "Usage: `basename $0` [Release|Debug]"
    exit 0
fi

if [ "$1" == "--help" ]; then
    echo "Usage: `basename $0` [Release|Debug]"
    exit 0
fi

# SET DEFAULT BUILD MODE IF NON GIVEN
if [ "$BUILD_MODE" = "" ]; then
    BUILD_MODE=$DEFAULT_BUILD_MODE
fi

# CHECK IF PROJUCER IS INSTALLED
if ! [ -x "$(command -v Projucer)" ]; then
    echo 'Error: Projucer is not installed.' >&2
    exit 1
fi

# RESAVE PROJECT
echo "Resaving $JUCE_PROJECT_NAME.jucer project"
Projucer --resave $JUCE_PROJECT_NAME.jucer

# BUILD
echo "Building $JUCE_PROJECT_NAME in $BUILD_MODE mode"
cd Builds/LinuxMakefile/
make CONFIG="$BUILD_MODE" -j$(nproc)

# CHECK IF VST DIRECTORY EXISTS
if [ -d "$VST_DIRECTORY" ]; then
    # Control will enter here if $DIRECTORY exists.
    echo "$VST_DIRECTORY already created"
else
    echo "Creating $VST_DIRECTORY"
    mkdir $VST_DIRECTORY
fi

# INSTALL VST BINARY
echo "Install to $VST_DIRECTORY"
cp build/$JUCE_PROJECT_NAME.so $VST_DIRECTORY


