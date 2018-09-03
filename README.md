[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

# tobanteAudio - modEQ

EQ audio effects plugin with modulation. Used as a playground to learn the [JUCE](https://github.com/WeAreRoli/JUCE) framework and modern C++.

## Build

### Dependencies

- [JUCE](https://github.com/WeAreRoli/JUCE)
- Steinberg [vst-sdk](https://github.com/steinbergmedia/vst3sdk)

### Windows

- Download & Install Visual Studio 2017
- Build Projucer from JUCE
- Open modEQ.jucer with Projucer
- Set path for vst sdk and juce modules
- Open in Visual Studio 2017 and select "Release"

### macOS

- Download & Install Xcode
- Build Projucer from JUCE
- Open modEQ.jucer with Projucer
- Set path for vst sdk and juce modules
- Open in Xcode and select "Release"

### Linux

- Build Projucer from JUCE
- Open modEQ.jucer with Projucer
- Set path for vst sdk and juce modules

```sh
cd $PROJECT_ROOT
cd Build/LinuxMakefile
make config=Release -j8
cp build/modEQ.so ~/.vst
```

# Acknowledgment

The project started with the code from Daniel Walz's github repository [ffAudio/Frequalizer](https://github.com/ffAudio/Frequalizer), which was released under the BSD 3-Clause license.
