# Modern Notify Icon

## Overview
![](gallery/overview.png)

## Features

Most features require minimum version of Windows 10 1603.

- Native Windows 10/11 look
- Windows notifications
- High dpi support
- Right-to-left text support
- Light/Dark/High Contrast theme support
- Fallback compatibility mode for Windows 7/8/8.1/10(pre 1603)

## How to use

You can use either pre-build static library or build library yourself.

### Requirements

- C++ compiler (std:c++17 or later) [only MSVC, other compiler not tested]

### To use pre-build library

1. Download or clone this repository
2. Add additional include directory: ```ModernNotifyIcon/include```
3. Add additional library directory: ```ModernNotifyIcon/lib/$(Platform)/$(Configuration)```
4. Add ```ModernNotifyIcon.lib``` to linker input
5. Include ```mni/NotifyIcon.hpp``` in your source

## Building

### Requirements

1. Minimum OS of Windows 10
2. Visual Studio 2022 with MSVC v143 and Windows SDK

### To build project

1. Open ```ModernNotifyIcon.sln``` and build project **ModernNotifyIcon**.
2. (Optional) Build **Demo** project

On successful build in will output library files in ```ModernNotifyIcon/lib``` directory.</br>
Demo executable will be placed in ```ModernNotifyIcon/bin``` directory.

Pre-build static library is built on Windows 10 21H2.

## Documentation

API documentation can be found [here](docs/README.md).

## License

The MIT License
