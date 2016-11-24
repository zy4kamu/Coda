# Object Coda
#
# Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
#
# Authors: Andrey Moiseenko <moiseenko.a@samsung.com>,
#          Alexander Romanenko <Alexander Romanenko>, 
#          Irina Piontkovskaya <p.irina@samsung.com>, 
#          Manh Ha Le <mh.le@samsung.com>,
#          Mikhail Kudinov <m.kudinov@samsung.com>, 
#          Stepan Muzychka <s.muzychka@samsung.com>  

# src/regexptools

set(TARGET_NAME regexptools)
set(regexptools_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(regexptools_SOURCES
    ${regexptools_SOURCE_DIR}/RegExp.cpp
)

file(GLOB boost_regex_SRC
    ${regexptools_SOURCE_DIR}/boost_regex/libs/regex/src/*.cpp
)

set(regexptools_HEADERS
    ${regexptools_SOURCE_DIR}/RegExp.h
)

#echo ${PROJECT_ROOT_DIR}

include_directories(
    ${regexptools_SOURCE_DIR}/boost_regex
    ${PROJECT_ROOT_DIR}/utils/Tools
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${regexptools_SOURCES} ${regexptools_HEADERS} ${boost_regex_SRC})
target_link_libraries(${TARGET_NAME} tools)
add_executable(${TARGET_NAME}-app ${regexptools_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")

