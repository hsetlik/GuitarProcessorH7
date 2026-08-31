cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME GuitarProcessorFirmware)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(CXX C ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/Core/Src)
set(DSP_DIR ${CMAKE_CURRENT_LIST_DIR}/Drivers/CMSIS/DSP)
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${SRC_DIR}/tlv320aic3204.c
    ${SRC_DIR}/PedalState.c
    ${SRC_DIR}/Button.c
    ${SRC_DIR}/FxAlgorithm.cpp
    ${SRC_DIR}/FxProcessor.cpp
    ${SRC_DIR}/DelayLine.cpp
    ${SRC_DIR}/Dattorro.cpp
    ${SRC_DIR}/AXISRAMPool.cpp
    ${SRC_DIR}/DTCMPool.cpp
    ${SRC_DIR}/Schroeder.cpp
    ${SRC_DIR}/Freeverb.cpp
    ${SRC_DIR}/NormRange.cpp
    ${DSP_DIR}/Source/BasicMathFunctions/BasicMathFunctions.c
    ${DSP_DIR}/Source/CommonTables/CommonTables.c
    ${DSP_DIR}/Source/FastMathFunctions/FastMathFunctions.c
    ${DSP_DIR}/Source/FilteringFunctions/FilteringFunctions.c
    ${DSP_DIR}/Source/TransformFunctions/TransformFunctions.c
)

set(DSP_HOT_SOURCES
    ${SRC_DIR}/Dattorro.cpp
    ${SRC_DIR}/DelayLine.cpp
    ${SRC_DIR}/Schroeder.cpp
    ${SRC_DIR}/Freeverb.cpp
    ${SRC_DIR}/FxAlgorithm.cpp
    ${SRC_DIR}/FxProcessor.cpp
    # add the CMSIS-DSP umbrella .c files here too if you want them at -O3
)
set_source_files_properties(${DSP_HOT_SOURCES} PROPERTIES
    COMPILE_OPTIONS "-O3;-ffp-contract=fast;-funroll-loops")

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${CMAKE_CURRENT_LIST_DIR}/Core/Inc
    ${CMAKE_CURRENT_LIST_DIR}/Drivers/CMSIS/DSP/Include
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
    ARM_MATH_LOOPUNROLL
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
