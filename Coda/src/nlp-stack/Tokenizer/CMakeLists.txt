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
    ${tokenizer_subfolders}
    ${PROJECT_ROOT_DIR}/utils/FileManager
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex/
    ${PROJECT_ROOT_DIR}/utils/RegexpTools/
)

add_library(${project_name} ${LIBRARY_TYPE} ${sources} ${headers})
target_link_libraries(${project_name} tools regexptools file-manager)

add_executable(${project_name}-app main.cpp)
target_link_libraries(${project_name}-app ${project_name})
