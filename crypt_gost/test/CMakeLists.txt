project(test)

if(ENABLE_TEST)
    find_package(GTest REQUIRED)
    include(GoogleTest)

    set(CORE_TEST_SOURCES core_test/allocator_test.cpp
                          core_test/math_addition_test.cpp
                          core_test/math_multiplication_test.cpp)

    set(CRYPTO_TEST_SOURCES crypto/hash/gost_2012_256_test.cpp)

    set(SOURCES main.cpp
                ${CORE_TEST_SOURCES}
                ${CRYPTO_TEST_SOURCES})

    add_executable(${PROJECT_NAME} ${SOURCES})
    target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} allocator hash pthread)

    add_custom_target(  leak-check
                        COMMAND valgrind --num-callers=25 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}
                        VERBATIM )

    add_custom_target(  check
                        COMMAND ${PROJECT_NAME}
                        VERBATIM )
endif()
