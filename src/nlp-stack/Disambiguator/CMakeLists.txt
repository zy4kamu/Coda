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
    ${crf_subfolders}
    ${disambiguator_subfolders}
    ${tokenizer_subfolders}
    ${dictionary_subfolders}

    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Clib
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/EventHandler
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer

    ${PROJECT_ROOT_DIR}/utils/FileManager
    ${PROJECT_ROOT_DIR}/utils/TagsetConverter
    ${PROJECT_ROOT_DIR}/utils/Tools

    ${PROJECT_ROOT_DIR}/nlp-stack/
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core
)

add_library(${project_name} ${LIBRARY_TYPE} ${sources} ${headers})
target_link_libraries(${project_name} tools file-manager optimization crf dictionary tokenizer tagset-converter)

add_executable(${project_name}-app main.cpp)
target_link_libraries(${project_name}-app ${project_name})
