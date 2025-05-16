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
   # "src/software_1/*.c"
   # "src/software_1/*.h"
    "src/base/*.c"
    "src/base/*.h"
)
target_sources(${EXECUTABLE_NAME} PRIVATE ${SRC_FILES})


# Configure SDL by calling its CMake file.
set(SDL_STATIC ON CACHE BOOL "Build SDL as a static library" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Do not build SDL as a shared library" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Do not build SDL tests" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Do not build SDL examples" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "Disable SDL install step" FORCE)
add_subdirectory("src/vendors/SDL" EXCLUDE_FROM_ALL)

add_subdirectory("src/vendors/SPIRV-Headers")

set(SPIRV_SKIP_TESTS ON CACHE BOOL "Skip building SPIRV-Tools tests" FORCE)
set(SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "Skip building SPIRV executable and tests along with the library" FORCE)
set(SKIP_SPIRV_TOOLS_INSTALL ON CACHE BOOL "Skip SPIRV tools installation" FORCE)
set(SPIRV_WERROR OFF CACHE BOOL "Do not enable error on warning" FORCE)
# We only need SPIRV-Tools-opt for glslang that also depends on SPIRV-Tools-static.
# Exclude all other targets from the build to save build time
add_subdirectory("src/vendors/SPIRV-Tools")
set(SPIRV_TOOLS_EXCLUDE_TARGETS SPIRV-Tools-link SPIRV-Tools-lint SPIRV-Tools-reduce SPIRV-Tools-shared)
set_target_properties(${SPIRV_TOOLS_EXCLUDE_TARGETS} PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

set(GLSLANG_TESTS_DEFAULT OFF CACHE BOOL "Do not build glslang tests." FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Do not build glslang as a shared lib." FORCE)
set(ENABLE_OPT ON CACHE BOOL "Enable optimization." FORCE) # if built for web this must off stated on glslang readme
set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "Turn off some standalone binary tools for glslang." FORCE)
set(ENABLE_SPVREMAPPER OFF CACHE BOOL "Do not build SPIRV remapper" FORCE)
set(GLSLANG_ENABLE_INSTALL OFF CACHE BOOL "Disable installation option for glslang." FORCE)
add_subdirectory("src/vendors/glslang")

target_link_libraries(${EXECUTABLE_NAME} PRIVATE 
    SDL3::SDL3-static # If using satelite libraries, SDL must be the last item in the list.
    glslang
    #spirv-cross-c
)