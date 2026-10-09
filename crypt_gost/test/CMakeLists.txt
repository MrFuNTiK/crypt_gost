project(test)

if(ENABLE_TEST)
    find_package(GTest REQUIRED)
    include(GoogleTest)

    add_subdirectory(unit)
    add_subdirectory(kat)
endif()
