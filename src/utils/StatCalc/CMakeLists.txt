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

    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Clib
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/EventHandler
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer

    ${PROJECT_ROOT_DIR}/nlp-stack/Lemmatizer
    ${PROJECT_ROOT_DIR}/nlp-stack/SentenceSplitter/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/PorterStemmer
    ${PROJECT_ROOT_DIR}/nlp-stack/PorterStemmer/include
    ${PROJECT_ROOT_DIR}/nlp-stack/Dictionary

    ${PROJECT_ROOT_DIR}/utils/Eigen
    ${PROJECT_ROOT_DIR}/utils/RegexpTools
    ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex/
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/FileManager
    ${PROJECT_ROOT_DIR}/utils/TagsetConverter
)

add_library(${project_name} ${LIBRARY_TYPE} ${sources} ${headers})
target_link_libraries(${project_name} tools dictionary lemmatizer tokenizer
    disambiguator regexptools sentence-splitter porterstemmer)

add_executable(${project_name}-app main.cpp)
target_link_libraries(${project_name}-app ${project_name})
