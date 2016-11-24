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

# src/NLUPhoenix
# retrieval-app demo application

add_subdirectory(sqliteClient)

set(TARGET_NAME NLUPhoenix)
set(NLUPhoenix_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(NLUPhoenix_SOURCES
    ${NLUPhoenix_SOURCE_DIR}/CommandInterpreter.cpp
    ${NLUPhoenix_SOURCE_DIR}/dataTools.cpp
    ${NLUPhoenix_SOURCE_DIR}/DbTables.cpp
    ${NLUPhoenix_SOURCE_DIR}/FilledFrame.cpp
    ${NLUPhoenix_SOURCE_DIR}/Frame.cpp
    ${NLUPhoenix_SOURCE_DIR}/Grammar.cpp
    ${NLUPhoenix_SOURCE_DIR}/GrammarNet.cpp
    ${NLUPhoenix_SOURCE_DIR}/main_grammar_test.cpp
    ${NLUPhoenix_SOURCE_DIR}/main_test.cpp
    ${NLUPhoenix_SOURCE_DIR}/NetChart.cpp
    ${NLUPhoenix_SOURCE_DIR}/NetEvaluator.cpp
    ${NLUPhoenix_SOURCE_DIR}/Parse.cpp
    ${NLUPhoenix_SOURCE_DIR}/PhoenixParser.cpp
    ${NLUPhoenix_SOURCE_DIR}/PhoenixSlotGraph.cpp
    ${NLUPhoenix_SOURCE_DIR}/QueryAssembler.cpp
    ${NLUPhoenix_SOURCE_DIR}/ResponseAssembler.cpp
    ${NLUPhoenix_SOURCE_DIR}/SlotTransformer.cpp
    ${NLUPhoenix_SOURCE_DIR}/SynTagRusTagInterpreter.cpp
    ${NLUPhoenix_SOURCE_DIR}/TemplateParser.cpp
    ${NLUPhoenix_SOURCE_DIR}/Word.cpp
    ${NLUPhoenix_SOURCE_DIR}/WORKER.cpp
)

set(NLUPhoenix_HEADERS
    ${NLUPhoenix_SOURCE_DIR}/CommandInterpreter.h
    ${NLUPhoenix_SOURCE_DIR}/commonExceptions.h
    ${NLUPhoenix_SOURCE_DIR}/dataTools.h
    ${NLUPhoenix_SOURCE_DIR}/debug.h
    ${NLUPhoenix_SOURCE_DIR}/FilledFrame.h
    ${NLUPhoenix_SOURCE_DIR}/Frame.h
    ${NLUPhoenix_SOURCE_DIR}/Grammar.h
    ${NLUPhoenix_SOURCE_DIR}/GrammarNet.h
    ${NLUPhoenix_SOURCE_DIR}/NetChart.h
    ${NLUPhoenix_SOURCE_DIR}/NetEvaluator.h
    ${NLUPhoenix_SOURCE_DIR}/Parse.h
    ${NLUPhoenix_SOURCE_DIR}/Phoenix.h
    ${NLUPhoenix_SOURCE_DIR}/PhoenixParser.h
    ${NLUPhoenix_SOURCE_DIR}/PhoenixSlotGraph.h
    ${NLUPhoenix_SOURCE_DIR}/QueryAssembler.h
    ${NLUPhoenix_SOURCE_DIR}/ResponseAssembler.h
    ${NLUPhoenix_SOURCE_DIR}/SlotTransformer.h
    ${NLUPhoenix_SOURCE_DIR}/SynTagRusTagInterpreter.h
    ${NLUPhoenix_SOURCE_DIR}/TemplateParser.h
    ${NLUPhoenix_SOURCE_DIR}/tests_classes.h
    ${NLUPhoenix_SOURCE_DIR}/Word.h
    ${NLUPhoenix_SOURCE_DIR}/WORKER.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/algorithms/CRF
    ${PROJECT_ROOT_DIR}/nlp-stack/Dictionary
    ${PROJECT_ROOT_DIR}/utils/RegexpTools
    ${PROJECT_ROOT_DIR}/applications/TimexNormalizer
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/FileManager
    ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex/
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Disambiguator
    ${PROJECT_ROOT_DIR}/algorithms/Phoenix/NLUPhoenix/sqliteClient
    ${PROJECT_ROOT_DIR}/algorithms/Optimization
    ${PROJECT_ROOT_DIR}/utils/TagsetConstants
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${NLUPhoenix_SOURCES} ${NLUPhoenix_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL NLUPhoenix_EXPORTS)
add_executable(${TARGET_NAME}-app ${NLUPhoenix_SOURCE_DIR}/main_grammar_test.cpp)
target_link_libraries(${TARGET_NAME} sqlite-client tokenizer dictionary file-manager regexptools tools disambiguator timexnormalizer tagsetconstants)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
