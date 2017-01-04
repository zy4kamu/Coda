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

set(project_name syntax-parser)

include_directories(
    ${crf_subfolders}
    ${disambiguator_subfolders}
    ${syntax_parser_subfolders}

    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Clib
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/EventHandler
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer

    ${PROJECT_ROOT_DIR}/nlp-stack/
    ${PROJECT_ROOT_DIR}/nlp-stack/Dictionary/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/SentenceSplitter/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core

    ${PROJECT_ROOT_DIR}/utils/TagsetConverter
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/FileManager
)

set(CMAKE_INCLUDE_SYSTEM_FLAG_${lang} "-isystem ")
include_directories(
    SYSTEM ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex
)

add_library(${project_name} ${LIBRARY_TYPE} ${sources} ${headers})
target_link_libraries(${project_name} tools file-manager crf sentence-splitter tokenizer disambiguator optimization gomp)

add_executable(${project_name}-app main.cpp)
target_link_libraries(${project_name}-app ${project_name})
