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

set(TARGET_NAME sentence-splitter)
set(spitter_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(splitter_SOURCES
    ${spitter_SOURCE_DIR}/Core/SSplitterCreator.cpp
    ${spitter_SOURCE_DIR}/Core/SentenceSplitter/sentence-splitter.cpp
    ${spitter_SOURCE_DIR}/Core/SentenceSplitter/ssplitter-feature-extractor.cpp
    ${spitter_SOURCE_DIR}/Core/SentenceSplitter/logistic-regression.cpp
    ${spitter_SOURCE_DIR}/Core/SentenceSplitter/ssplitter-trainer.cpp
    ${spitter_SOURCE_DIR}/Core/Exceptions/LanguageNotRegisteredException.cpp
)

set(spitter_HEADERS
    ${spitter_SOURCE_DIR}/Core/ISentenceSplitter.h
    ${spitter_SOURCE_DIR}/Core/SSplitterCreator.h
    ${spitter_SOURCE_DIR}/Core/SentenceSplitter/sentence-splitter.h
    ${spitter_SOURCE_DIR}/Core/SentenceSplitter/ssplitter-feature-extractor.h
    ${spitter_SOURCE_DIR}/Core/SentenceSplitter/logistic-regression.h
    ${spitter_SOURCE_DIR}/Core/SentenceSplitter/ssplitter-trainer.h
    ${spitter_SOURCE_DIR}/Core/Exceptions/ssplitter-exceptions.h
    ${spitter_SOURCE_DIR}/Core/Exceptions/LanguageNotRegisteredException.h
) 
#	${spitter_SOURCE_DIR}/dirent.h

include_directories(
    ${spitter_SOURCE_DIR}/Core
    ${spitter_SOURCE_DIR}/Core/Exceptions
    ${spitter_SOURCE_DIR}/Core/SentenceSplitter

    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Clib
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/EventHandler
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer

    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/Tools/Locale
    ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex/
    ${PROJECT_ROOT_DIR}/utils/RegexpTools/
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${splitter_SOURCES} ${spitter_HEADERS})
target_link_libraries(${TARGET_NAME} tools regexptools optimization)
add_executable(${TARGET_NAME}-app ${spitter_SOURCE_DIR}/main.cpp)

target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")

