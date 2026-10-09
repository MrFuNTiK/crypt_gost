project(kat_test)

set(COMMON_SOURCES common/kat_environment.cpp)

set(TESTS_SORCES test/gost_34_11_2012.cpp)

set(SOURCES main.cpp
            ${COMMON_SOURCES}
            ${TESTS_SORCES}
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} allocator hash pthread)

add_custom_target(kat-check
                  COMMAND ${PROJECT_NAME} --config-ini ${CMAKE_CURRENT_SOURCE_DIR}/data/
                  DEPENDS ${PROJET_NAME}
                  VERBATIM )
