project(xclm)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add single headers to project structure
FILE(GLOB_RECURSE HeaderFiles "*.h")
add_custom_target(headers SOURCES ${HeaderFiles})

include_directories(.)

find_package(Boost 1.57.0 COMPONENTS system program_options filesystem REQUIRED)

if(Boost_FOUND)
    aux_source_directory(. SRC_LIST)
    aux_source_directory(hashes SRC_LIST)
    aux_source_directory(runner SRC_LIST)
    aux_source_directory(version SRC_LIST)
    add_executable(${PROJECT_NAME} ${SRC_LIST})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "boost library not found.")
endif()
