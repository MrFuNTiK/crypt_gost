project(test)

if(ENABLE_TEST)
    find_package(GTest REQUIRED)
    include(GoogleTest)

    add_executable(${PROJECT_NAME} core_test/main.cpp
                                   core_test/allocator_test.cpp
                                   core_test/math_addition_test.cpp
                                   core_test/math_multiplication_test.cpp)
    target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} allocator pthread)

    add_custom_target(  leak-check
                        COMMAND valgrind --num-callers=25 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}
                        VERBATIM )

    add_custom_target(  check
                        COMMAND ${PROJECT_NAME}
                        VERBATIM )
endif()
