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

# src/chanking

set(TARGET_NAME chunking)
set(chunking_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(chunking_SOURCES
    ${chunking_SOURCE_DIR}/ChunkingProcessor.cpp
    ${chunking_SOURCE_DIR}/SyntaxChunker.cpp
)

set(chunking_HEADERS
    ${chunking_SOURCE_DIR}/ChunkingProcessor.h
    ${chunking_SOURCE_DIR}/SyntaxChunker.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/algorithms/CRF
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Chain
    ${PROJECT_ROOT_DIR}/algorithms/CRF/ChainTransformer
    ${PROJECT_ROOT_DIR}/algorithms/CRF/IO
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Inference
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Model
    ${PROJECT_ROOT_DIR}/algorithms/CRF/PossibleStateFinder
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Learner
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Settings
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Tester
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Regularization
    ${PROJECT_ROOT_DIR}/algorithms/CRF/ViterbiNode
    ${PROJECT_ROOT_DIR}/algorithms/Optimization

    ${PROJECT_ROOT_DIR}/nlp-stack/Dictionary
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Disambiguator
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core

    ${PROJECT_ROOT_DIR}/utils/FileManager
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/Tools/Strings

    ${PROJECT_ROOT_DIR}/utils/TagsetConstants
    ${PROJECT_ROOT_DIR}/utils/TagsetConverter
)

if (MSVC)
    add_definitions(-DCONFIG_PATH="${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Config_Windows/")
    add_definitions(-DCHUNKER_CONFIG_PATH="${PROJECT_ROOT_DIR}/Chunking/Config_Windows/")
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-DCONFIG_PATH=\"${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Config_Linux/\")
    add_definitions(-DCHUNKER_CONFIG_PATH=\"${PROJECT_ROOT_DIR}/Chunking/Config_Linux/\")
endif()

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${chunking_SOURCES} ${chunking_HEADERS})
target_link_libraries(${TARGET_NAME} tools tokenizer tagsetConverter csvManager crf disambiguator tagsetconstants dictionary)
add_executable(${TARGET_NAME}-app ${chunking_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")

