cmake_minimum_required(VERSION 3.10)
project(Editor)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -Werror -std=c++20 -DDEBUG_A)

# Add object files:
file(GLOB_RECURSE EditorSources src/*.cpp)

# Header files location
set(EditorHeaders include/)


# Auxiliary libs should be compiled separately
set(LoggerHeader aux/logger)
# e.g add_library(<libName>)

# Glue togheter all the headers
set(Headers 
            ${EditorHeaders}
            ${LoggerHeader})

add_executable(Editor editor.cpp ${EditorSources})

target_include_directories(Editor PUBLIC ${Headers})



# Install phase

install(TARGETS Editor DESTINATION ${CMAKE_SOURCE_DIR}/bin/)
