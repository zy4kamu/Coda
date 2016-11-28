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

include (../../common_includes.cmake)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/boost_regex
    ${PROJECT_ROOT_DIR}/utils/Tools
)

file(GLOB boost_regex_src
    ${CMAKE_CURRENT_LIST_DIR}/boost_regex/libs/regex/src/*.cpp
)

add_library(${project_name} ${LIBRARY_TYPE} ${sources} ${headers} ${boost_regex_src})
target_link_libraries(${project_name} tools)

add_executable(${project_name}-app main.cpp)
target_link_libraries(${project_name}-app ${project_name})
