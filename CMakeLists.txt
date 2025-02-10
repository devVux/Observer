cmake_minimum_required(VERSION 3.10)

project(ObserverPattern)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories("include/")

set(HEADERS
    "include/Observer.h"
)

set(SOURCES
    main.cpp
)

add_executable(main ${SOURCES} ${HEADERS})

enable_testing()
add_subdirectory(tests)