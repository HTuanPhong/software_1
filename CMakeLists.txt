cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Declare the project
project(software_1)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/result)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/result)

# prevent installing to system directories. 
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")

# Set the name of the executable
set(EXECUTABLE_NAME ${PROJECT_NAME})

add_executable(${EXECUTABLE_NAME})

#target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
file(GLOB SRC_FILES CONFIGURE_DEPENDS    
    "src/software_1/*.c"
    "src/software_1/*.h"
    "src/base/*.c"
    "src/base/*.h"
)
target_sources(${EXECUTABLE_NAME} PRIVATE ${SRC_FILES})

#target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_20)

# Configure SDL by calling its CMake file.
# we use EXCLUDE_FROM_ALL so that its install targets and configs don't
# pollute upwards into our configuration.
set(BUILD_SHARED_LIBS ON)
set(SDL_SHARED ON)
add_subdirectory("src/vendors/SDL3" EXCLUDE_FROM_ALL)

set(SDLTTF_VENDORED ON) # tell SDL_ttf to build its own dependencies
add_subdirectory("src/vendors/SDL_ttf" EXCLUDE_FROM_ALL)

set(SDLIMAGE_VENDORED ON) # tell SDL_image to build its own dependencies
set(SDLIMAGE_AVIF OFF)
add_subdirectory("src/vendors/SDL_image" EXCLUDE_FROM_ALL)

set(SDLSHADERCROSS_VENDORED ON)
add_subdirectory("src/vendors/SDL_shadercross" EXCLUDE_FROM_ALL)

target_link_libraries(${EXECUTABLE_NAME} PUBLIC 
    SDL3_shadercross::SDL3_shadercross
    SDL3_image::SDL3_image	
    SDL3_ttf::SDL3_ttf
    SDL3::SDL3 # If using satelite libraries, SDL must be the last item in the list.
)

set(RESOURCE_FILES "src/logo.png")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES 
    RESOURCE "${RESOURCE_FILES}"
)