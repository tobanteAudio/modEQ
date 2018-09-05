#!/bin/sh

JUCE_PROJECT_NAME=modEQ
BUILD_MODE=$1
VST_DIRECTORY=~/.vst/tobanteAudio

if [ "$BUILD_MODE" = "" ]
  then BUILD_MODE=Release
fi

if ! [ -x "$(command -v Projucer)" ]; then
  echo 'Error: Projucer is not installed.' >&2
  exit 1
fi

echo "Resaving $JUCE_PROJECT_NAME.jucer project"
Projucer --resave $JUCE_PROJECT_NAME.jucer

echo "Building $JUCE_PROJECT_NAME in $BUILD_MODE mode"
cd Builds/LinuxMakefile/

make CONFIG="$BUILD_MODE" -j$(nproc)


if [ -d "$VST_DIRECTORY" ]; then
  # Control will enter here if $DIRECTORY exists.
  echo "$VST_DIRECTORY already created"
else
echo "Creating $VST_DIRECTORY"
  mkdir $VST_DIRECTORY
fi


echo "Install to $VST_DIRECTORY"
cp build/$JUCE_PROJECT_NAME.so $VST_DIRECTORY


