cmake_minimum_required(VERSION 3.16)

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
target_compile_features(${EXECUTABLE_NAME} PUBLIC c_std_11)
file(GLOB SRC_FILES CONFIGURE_DEPENDS
    "src/main.c"    
    "src/software_1/*.c"
    "src/software_1/*.h"
    "src/base/*.c"
    "src/base/*.h"
)
target_sources(${EXECUTABLE_NAME} PRIVATE ${SRC_FILES})


# Configure SDL by calling its CMake file.
# we use EXCLUDE_FROM_ALL so that its install targets and configs don't
# pollute upwards into our configuration.
set(SDL_STATIC ON CACHE BOOL "Build SDL as a static library" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Do not build SDL as a shared library" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Do not build SDL tests" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Do not build SDL examples" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "Disable SDL install step" FORCE)
add_subdirectory("src/vendors/SDL3" EXCLUDE_FROM_ALL)

# set(SDLTTF_VENDORED ON) # tell SDL_ttf to build its own dependencies
# add_subdirectory("src/vendors/SDL_ttf" EXCLUDE_FROM_ALL)

# set(SDLIMAGE_VENDORED ON) # tell SDL_image to build its own dependencies
# set(SDLIMAGE_AVIF OFF)
# add_subdirectory("src/vendors/SDL_image" EXCLUDE_FROM_ALL)

# set(SDLSHADERCROSS_VENDORED ON)
# add_subdirectory("src/vendors/SDL_shadercross" EXCLUDE_FROM_ALL)

target_link_libraries(${EXECUTABLE_NAME} PUBLIC 
    SDL3::SDL3-static # If using satelite libraries, SDL must be the last item in the list.
)

set(RESOURCE_FILES "src/logo.png")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES 
    RESOURCE "${RESOURCE_FILES}"
)