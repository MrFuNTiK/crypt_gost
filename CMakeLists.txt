cmake_minimum_required(VERSION 3.30)

project(crypt_gost LANGUAGES C CXX)

include(cmake/system.cmake)

set(CMAKE_CXX_STANDARD 17)
set(RESULT_DIRECTORY ${CMAKE_SOURCE_DIR}/_result)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RESULT_DIRECTORY}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RESULT_DIRECTORY}/lib)

include_directories(include crypt_gost/internal_include)
add_compile_options(-Wall -Wextra -Werror)

option(ENABLE_TEST CACHE ON)
determine_byte_ordering()

add_subdirectory(crypt_gost)
add_executable(main main.cpp)
target_link_libraries(main allocator)
