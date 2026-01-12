# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an LVGL simulator project for developing STM32 embedded GUI applications on PC. It allows running LVGL-based graphics code on Linux/Windows/macOS using SDL2, with optional FreeRTOS support to better simulate embedded behavior.

The project targets a CANLineX2 device with an 800x480 display.

## Build Commands

```bash
# Standard build (without FreeRTOS)
cmake -B build
cmake --build build -j

# Build with FreeRTOS enabled
cmake -B build -DUSE_FREERTOS=ON
cmake --build build -j

# Run the application
./bin/main
```

### Optional Features

```bash
# Enable AddressSanitizer (Debug builds)
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DASAN=ON

# Enable optional libraries
cmake -B build -DLV_USE_DRAW_SDL=ON -DLV_USE_LIBPNG=ON -DLV_USE_FREETYPE=ON
```

### Dependencies

- SDL2 (required): `sudo apt install libsdl2-dev`
- Optional: FreeType, FFmpeg, libpng, libjpeg-turbo

## Architecture

### Main Components

- **lvgl/**: LVGL graphics library (submodule)
- **FreeRTOS/**: FreeRTOS kernel (submodule, optional)
- **CANLineX2Graphics/**: Application UI layer - display state machine, windows, widgets
- **CANLineX2Interface/**: Hardware abstraction layer - UART, timers, configuration handling

### Library Structure

Two shared libraries are built:
- `LVGLInterfaceLIB`: Low-level drivers (UARTDriver, TimerLib, ConfigurationHandler, ServerPattern)
- `LVGLGraphicsLIB`: UI components (DisplayStateMachine, MainFrame, windows, charts)

### Entry Points

- `src/main.c`: Standard main loop (non-FreeRTOS)
- `src/freertos_main.c`: FreeRTOS-based main

### Platform Abstraction

Code uses `#ifdef __linux__` / `#ifndef __linux__` for platform-specific sections. STM32 HAL types are stubbed for Linux builds.

## Code Style

- Allman brace style (braces on new lines for functions and control statements)
- 2-space indentation, no tabs
- 120 character line limit
- Pointer alignment: `type *ptr` (right-aligned)
- Use `.clang-format` for formatting

## Key Patterns

- **State Machine**: `DisplayStateMachine.c` manages UI state transitions
- **Timeout Server**: `TimeoutServer/` provides software timer callbacks
- **Configuration**: `ConfigurationHandler.c` handles device configuration with CRC validation
- **UART Callbacks**: Register-based callback system in `UARTDriver.c` for multiple UART instances