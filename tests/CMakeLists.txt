
project(ObserverPatternTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/googletest" "${CMAKE_BINARY_DIR}/googletest")
enable_testing()	

include_directories("${CMAKE_SOURCE_DIR}/include")

add_executable(tests "test_observer.cpp")
target_link_libraries(tests gtest gtest_main)

add_test(NAME ObserverTests COMMAND tests)