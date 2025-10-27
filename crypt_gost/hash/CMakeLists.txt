project(hash)

add_library(${PROJECT_NAME} STATIC  gost_2012_256.cpp
                                    i_hash.cpp)
