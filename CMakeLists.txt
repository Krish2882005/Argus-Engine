cmake_minimum_required(VERSION 3.28)
project(ArgusEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

find_package(GTest CONFIG REQUIRED)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_library(argus_compiler_flags INTERFACE)

if(MSVC)
    target_compile_options(argus_compiler_flags INTERFACE 
        /W4 /WX /permissive- 
        /Zc:__cplusplus 
        /Zc:preprocessor 
        /utf-8 
        /Gw
        /wd4324
    )
    target_compile_definitions(argus_compiler_flags INTERFACE 
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
endif()

add_subdirectory(src/Core)
add_subdirectory(src/Compute)
add_subdirectory(src/SensorMock)
add_subdirectory(src/ComputeEngine)
add_subdirectory(tests)
add_subdirectory(benchmarks)
