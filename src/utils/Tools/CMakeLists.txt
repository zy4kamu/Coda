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

set(CMAKE_INCLUDE_SYSTEM_FLAG_${lang} "-isystem ")

include_directories(
    SYSTEM ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex
    tools_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}
)

add_library(${project_name} ${LIBRARY_TYPE} ${sources} ${headers})
target_link_libraries(${project_name})

add_executable(${project_name}-app main.cpp)
target_link_libraries(${project_name}-app ${project_name})
