cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(DNA_ALGORITHM)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CMAKE_BUILD_TYPE "cmake build type" "Debug")
option(BUILD_SHARED_LIBS "build shared library" OFF)
option(USE_LIBS "usage library" OFF)

set(CPP_LIB "src/Individual.cpp")

set(LIB_INCLUDE "include")

set(MAIN_INCLUDE "include")

if(${USE_LIBS})
    add_library(xxx)
    target_sources(xxx PRIVATE ${CPP_LIB})
    target_include_directories(xxx PUBLIC ${LIB_INCLUDE})
    add_executable(main src/main.cpp)
    target_link_libraries(main xxx)
else()
    aux_source_directory(src SRC)
    add_executable(main ${SRC})
    target_include_directories(main PUBLIC ${MAIN_INCLUDE})

endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(main PUBLIC IS_LINUX)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(main PUBLIC IS_WINDOWS)
endif()

find_package(PythonInterp REQUIRED)

