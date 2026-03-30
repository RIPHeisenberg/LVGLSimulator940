# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

LVGL v9.4.0 simulator for developing STM32 embedded GUI applications on PC. Runs LVGL-based graphics on Linux/Windows/macOS using SDL2, with optional FreeRTOS support. Targets a CANLineX2 gas detection device with an 800x480 display, supporting up to 128 sensors and 128 relays via CAN bus.

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

### Two Shared Libraries

- **`LVGLGraphicsLIB`** (`CANLineX2Graphics/`): UI layer — state machine, windows, widgets, charts, fonts
- **`LVGLInterfaceLIB`** (`CANLineX2Interface/`): Hardware abstraction — UART, timers, configuration, CRC, event system

### Entry Points

- `src/main.c`: Standard cooperative main loop (non-FreeRTOS)
- `src/freertos_main.c`: FreeRTOS task-based main — LVGL runs in dedicated task with `vTaskDelay`
- `src/APIFunctions.c`: Simulator-only stubs — loads config from `Configuration.inc` instead of hardware flash, declares global `App_t App`

### Main Loop Flow (non-FreeRTOS)

```
lv_init() → sdl_hal_init(800,480) → ChartData_init() → DisplayStateMachine_init()
Loop: TimeoutServer_handler() → ChartData_handler(1s) → lv_timer_handler() → DisplayStateMachine_handler() → sleep
```

### Display State Machine (SMF Framework)

`DisplayStateMachine.c` uses a State Machine Framework (`smf.c`) with entry/run/exit functions per state. `DisplayStateMachine_handler()` calls `smf_run_state()` each iteration.

**14 states:** `SM_DISPLAY_TEST`, `SM_MAIN_WINDOW`, `SM_CHART_VIEW`, `SM_INFO_WINDOW`, `SM_PASSWORD_WINDOW`, `SM_CLOCK_WINDOW`, `SM_SERVICE_WINDOW`, `SM_ALARM_HISTORY`, `SM_OUTPUT_TEST`, `SM_INHIBIT_WINDOW`, `SM_SENSOR_INFO_WINDOW`, `SM_RELAIS_INFO_WINDOW`, `SM_MAIN_VIEW_CONFIG`, `SM_SETUP_WINDOW`

Transitions are driven by each window's `_handler()` return value (`DisplayStates_t`), processed by `StateTransitionHandler()` → `smf_set_state()`.

### Window Module Pattern

Every window follows this convention:

```c
void WindowName_init(void);                    // Called once at startup
void WindowName_load(void);                    // Called on state entry — creates/shows UI
DisplayStates_t WindowName_handler(void);      // Called each loop — returns next state
```

State machine wiring in `DisplayStateMachine.c`:
```c
static void WindowEntry(void *o) { WindowName_load(); }
static void WindowRun(void *o) {
    DisplayStates_t next = WindowName_handler();
    StateTransitionHandler(&StateObject, next);
}
```

### Async Communication: TimeoutServer + ServerPattern

Request flow for hardware communication:

1. UI calls `TimeoutServer_request(HMIReadType_t, errorCallback, timeoutMs)` or `TimeoutServer_PayloadRequest(...)` for write operations
2. `ConfigurationHandler_handler()` processes queued requests, sends UART commands
3. On response: `ServerPattern_signalEvent(event, data)` triggers registered callbacks
4. On timeout: error callback invoked

**ServerPattern** is a pub-sub system (50 callback slots): `ServerPattern_registerCallback(event, callback)` / `ServerPattern_signalEvent(event, data)`

### ConfigurationHandler Key APIs

```c
bool ConfigurationHandler_Sensor_get(uint8_t index, Sensor_t *sensor);
bool ConfigurationHandler_Relay_get(uint8_t index, relay_t *relay);
int16_t ConfigurationHandler_SensorValue_get(uint16_t sensorIndex);
bool ConfigurationHandler_System_get(Sys_t **pSystem);
bool ConfigurationHandler_ReadRequest(HMIReadType_t ReadType, void *pPayload, int16_t payloadSize);
```

### Platform Abstraction

Code uses `#ifdef __linux__` / `#ifndef __linux__` for platform-specific sections. STM32 HAL types (e.g., `UART_HandleTypeDef`) are stubbed as empty structs for Linux builds. CRC8 validation protects configuration integrity.

## Rules

- **Do not compile or build the project.** Never run `cmake`, `make`, or any build commands. The user will build and test manually.

## Code Style

- Allman brace style (braces on new lines for functions and control statements)
- 2-space indentation, no tabs
- 120 character line limit
- Pointer alignment: `type *ptr` (right-aligned)
- Use `.clang-format` for formatting
