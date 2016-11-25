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

set(project_name file-manager)

include_directories(
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/Tools/Strings
)

add_library(${project_name} ${LIBRARY_TYPE} ${sources} ${headers})
target_link_libraries(${project_name} tools)

add_executable(${project_name}-app main.cpp)
target_link_libraries(${project_name}-app ${project_name})
