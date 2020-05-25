# Solution: Ultra
cmake_minimum_required (VERSION 3.15)
project ("Ultra" VERSION 0.0.1 DESCRIPTION "Ultra Spectrum" LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

# Projects Root
add_subdirectory(Source)
