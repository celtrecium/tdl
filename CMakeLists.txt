cmake_minimum_required(VERSION 3.16)
include(FetchContent)

project(tdl)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-std=c99 -pedantic -Wall -Wextra -Wsign-conversion -Wconversion -Wshadow)

if (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_SECURE_NO_DEPRECATE)
endif()

FetchContent_Declare(
    libsbvector
    GIT_REPOSITORY https://github.com/celtrecium/libsbvector.git
)

FetchContent_Declare(
    libu8string
    GIT_REPOSITORY https://github.com/celtrecium/libu8string.git
)

FetchContent_MakeAvailable(libsbvector libu8string)

include_directories(include/ ${libsbvector_SOURCE_DIR}/include/ ${libu8string_SOURCE_DIR}/include/)

file(GLOB
    SRCS "src/*.c")

add_library(${PROJECT_NAME} SHARED ${SRCS})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES include/tdl.h DESTINATION include)
install(DIRECTORY include/tdl DESTINATION include)
