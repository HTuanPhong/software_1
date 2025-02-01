add_subdirectory(./vendors)
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE software_1/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE vendors)