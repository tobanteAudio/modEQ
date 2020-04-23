# tobanteAudio - modEQ

EQ audio effects plugin with modulation. Supports **macOS**, **Windows** & **Linux** with **VST**, **AudioUnits** & **Standalone** builds.

Used as a playground to learn the [JUCE](https://github.com/WeAreRoli/JUCE) framework and modern C++.

[**Releases**](https://github.com/tobanteAudio/modEQ/releases)

[**Github Repository**](https://github.com/tobanteaudio/modEQ/)

[**Developer Documentation**](https://tobanteaudio.github.io/modEQ/)

## Status

|                                                      LICENSE                                                       |                                                          macOS                                                          |                                                                  Windows                                                                  |                                                        Linux                                                        |
| :----------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------------------: |
| [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) | [![Build Status](https://travis-ci.org/tobanteAudio/modEQ.svg?branch=master)](https://travis-ci.org/tobanteAudio/modEQ) | [![AppVeyor Build status](https://img.shields.io/appveyor/ci/tobanteAudio/modEQ.svg)](https://ci.appveyor.com/project/tobanteAudio/modEQ) | [![CircleCi](https://circleci.com/gh/tobanteAudio/modEQ.svg?style=svg)](https://circleci.com/gh/tobanteAudio/modEQ) |

## Screenshot

![tobanteAudio modEQ](doc/modEQ_screenshot_plugin.png)

## Clone

```sh
git clone --recursive https://github.com/tobanteAudio/modEQ.git
```

## Features

- 6 Band EQ
  - High Pass
  - Low Shelf
  - Band Pass
  - Peak
  - High Shelf
  - Low Pass
- Input & output analyzer
- Solo / bypass each band
- Control bands from Sliders or Frequency plot
  - Drag to set gain & frequency
  - Right click to select filter type
  - Double click to toggle bypass
  - Scroll wheel selects Q

### What doesn't work (yet)

- Modulation (I know it's in the name...)
- Bypass button

## Build

### Windows

- Download & Install Visual Studio 2017
- Build Projucer from JUCE
- Open & Resave modEQ.jucer with Projucer
- Open in Visual Studio 2017 and select "Release"

### macOS

- Download & Install Xcode
- Build Projucer from JUCE
- Open & Resave modEQ.jucer with Projucer
- Open in Xcode and select "Release"

### Linux

- Install dependencies
- Build Projucer from JUCE
- Resave modEQ.jucer with Projucer

```sh
cd $PROJECT_ROOT/Build/LinuxMakefile
make config=Release -j8
cp build/modEQ.so ~/.vst
```

## ToDo

- Modulation
  - Reactivate
  - Fix drawing of LFO
  - Add waveforms
- CI
  - Deploy binaries on tagged commit
  - Unit test runner

## License

This Project is released under the `GNU General Public License Version 3`. See the [license file](LICENSE.md) for details.

### External

#### JUCE

JUCE is free open source projects released under the `GPL v3`. See [JUCE License](https://github.com/WeAreROLI/JUCE/blob/master/LICENSE.md) for details.

#### ff_meters

ffAudio's ff_meters is released under the `BSD 3-Clause` license. See [ff_meters license](https://github.com/ffAudio/ff_meters/blob/master/LICENSE.md) for details.
