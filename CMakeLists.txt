# ---------------------------------------------------------------------------
# CSEN701 - Project Milestone 1
# Two-button LED control on a Raspberry Pi Pico (RP2040)
# ---------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.13)

# Ensure compile_commands.json is always generated for VS Code IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.1)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# Target board.
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in the Pico SDK.
include(pico_sdk_import.cmake)

project(MS_01_LED_Buttons C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise the Pico SDK.
pico_sdk_init()

# Executable and its source files.
add_executable(MS_01_LED_Buttons
    main.c
    keypad.c
)

# Link against the standard Pico library.
target_link_libraries(MS_01_LED_Buttons
    pico_stdlib
)

# No serial output is used in this milestone.
pico_enable_stdio_usb(MS_01_LED_Buttons 1)
pico_enable_stdio_uart(MS_01_LED_Buttons 0)

# Generate .uf2, .bin, .hex, and .map next to .elf.
pico_add_extra_outputs(MS_01_LED_Buttons)