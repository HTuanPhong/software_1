add_subdirectory(./vendors)
add_executable(${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
file(GLOB SRC_FILES CONFIGURE_DEPENDS    
    "./software_1/*.c"
    "./software_1/*.h"
    "./base/*.c"
    "./base/*.h"
)
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE vendors)