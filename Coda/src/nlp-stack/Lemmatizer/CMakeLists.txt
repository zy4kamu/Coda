# Object Cadenza
#
# Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
#
# Authors: Andrey Moiseenko <moiseenko.a@samsung.com>,
#          Alexander Romanenko <Alexander Romanenko>, 
#          Irina Piontkovskaya <p.irina@samsung.com>, 
#          Manh Ha Le <mh.le@samsung.com>,
#          Mikhail Kudinov <m.kudinov@samsung.com>, 
#          Stepan Muzychka <s.muzychka@samsung.com>  

# src/retrieval-app
# retrieval-app demo application

set(TARGET_NAME lemmatizer)
set(lemmatizer_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(lemmatizer_SOURCES
    ${lemmatizer_SOURCE_DIR}/lemmatizer.cpp
)

set(lemmatizer_HEADERS
    ${lemmatizer_SOURCE_DIR}/lemmatizer.h
    ${lemmatizer_SOURCE_DIR}/lemmatizer-interface.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Clib
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/EventHandler
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer

    ${PROJECT_ROOT_DIR}/nlp-stack/Dictionary
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Disambiguator
    ${PROJECT_ROOT_DIR}/nlp-stack/Lemmatizer
    ${PROJECT_ROOT_DIR}/nlp-stack/SentenceSplitter/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core

    ${PROJECT_ROOT_DIR}/utils/FileManager
    ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex/
    ${PROJECT_ROOT_DIR}/utils/RegexpTools/
    ${PROJECT_ROOT_DIR}/utils/TagsetConverter
    ${PROJECT_ROOT_DIR}/utils/Tools
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${lemmatizer_SOURCES} ${lemmatizer_HEADERS})
add_executable(${TARGET_NAME}-app ${lemmatizer_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools dictionary tokenizer disambiguator sentence-splitter)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
