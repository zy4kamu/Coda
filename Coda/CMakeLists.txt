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

cmake_minimum_required (VERSION 2.8)
project(Coda)

# sanity check
if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(SEND_ERROR "In-source builds are not allowed.")
endif()

# define linkage type
if (LINK_SHARED)
    set(LIBRARY_TYPE STATIC)
else()
    set(LIBRARY_TYPE SHARED)
endif()

# set compiler flags, etc.
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -c ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -std=c++11") #TODO:  -Wconversion -Weffc++")
add_definitions(-D__STDC_LIMIT_MACROS)

# set folder names
if (NOT CONFIG_ROOT_DIR)
    set(CONFIG_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/config)
endif()
message("config output folder: " ${CONFIG_ROOT_DIR})

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()
message("binary ouptut folder: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()
message("library output folder: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(MAIN_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR})

# set build type
if( NOT CMAKE_BUILD_TYPE )
    set(CMAKE_BUILD_TYPE "Debug")
endif()
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("Building Debug...")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -Wno-unknown-pragmas")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message("Building Release...")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -pthread -O3 -march=native -Wall -Wno-unused-result -fopenmp -funroll-loops -ffast-math") # Mikolov
else()
    message("Building DEFAULT...")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -Wno-unknown-pragmas")
endif()

# set common variables
# SERVER
if( NOT SMAUG_PATH )
    set(SMAUG_PATH /media/smaug/)
endif()
add_definitions(-DSERVER_PATH=\"${SMAUG_PATH}\")
#CSV MANAGER
add_definitions(-DCSV_MANAGER_CONFIG_PATH=\"${CONFIG_ROOT_DIR}/CSVManager/Config_Linux/\")
#CRF
add_definitions(-DCRF_TESTING_MODEL_OUTPUT_PATH=\"${CONFIG_ROOT_DIR}/CRF/Config_Linux/Model_Output.txt\")
add_definitions(-DCRF_CONFIG_FOLDER=\"${CONFIG_ROOT_DIR}/CRF/Config_Linux/\")
add_definitions(-DCRF_TEST_SET_PATH=\"${CONFIG_ROOT_DIR}/CRF/Config_Linux/TestSet.txt\")
add_definitions(-DCRF_TRAINING_SET_PATH=\"${CONFIG_ROOT_DIR}/CRF/Config_Linux/TrainingSet.txt\")
add_definitions(-DCRF_ALL_TAGS_FILE_PATH=\"${CONFIG_ROOT_DIR}/CRF/Config_Linux/AllTags.txt\")
# DECOMPOSER
add_definitions(-DDECOMPOSER=\"${CONFIG_ROOT_DIR}/Decomposer/\")
add_definitions(-DDECOMPOSER_CLASSIFIER=\"${CONFIG_ROOT_DIR}/Decomposer/350/\")
add_definitions(-DDECOMPOSER_GAPPS_GAMES=\"${CONFIG_ROOT_DIR}/Decomposer/GoogleAppsGames/\")
add_definitions(-DDECOMPOSER_GAPPS_NOTGAMES=\"${CONFIG_ROOT_DIR}/Decomposer/GoogleAppsNotGames/\")
# DICTIONARY
add_definitions(-DBIN_DIC_DATA_PATH=\"${CONFIG_ROOT_DIR}/Dictionary/Data/\")
#DISAMBIGUATOR
add_definitions(-DDISAMBIGUATOR_CONFIG_FOLDER_STRING=\"${CONFIG_ROOT_DIR}/Disambiguator/Config_Linux/\")
# EVENT_EXTRACTOR
add_definitions(-DEVENT_EXTRACTOR_CONFIG=\"${CONFIG_ROOT_DIR}/EventExtractor/model\")
# TIMEX_DISAMBIGUATOR
add_definitions(-DTIMEX_DISAMBIGUATOR_CONFIG_FOLDER=\"${CONFIG_ROOT_DIR}/TimexDisambiguator/\")
# GRAPHVIZ
add_definitions(-DGRAPHVIZ_PATH=\"\")
# KINOPOISK DEMO
add_definitions(-DKINOPOISK_DEMO_CONFIG_FOLDER=\"${CONFIG_ROOT_DIR}/KinopoiskDemo/Config_Linux/\")
# NAME_ENTITY_RECOGNIZER
add_definitions(-DNE_RECOGNIZER_CONFIG_PATH=\"${CONFIG_ROOT_DIR}/NER/Config_Linux/\")
# NLP FILE PROCESSOR
add_definitions(-DNLP_FILE_PROCESSOR_CONFIG_FOLDER=\"${CONFIG_ROOT_DIR}/NLPFileProcessor/Config_Linux/\")
# TEXTCLASSIFICATIONEN
add_definitions(-DTEXT_CLASSIFICATION_EN_DATA_PATH=\"${CONFIG_ROOT_DIR}/TextClassificationEn/Data/\")
# TOKENIZER
add_definitions(-DTOKENIZER_CONFIG_FOLDER=\"${CONFIG_ROOT_DIR}/Tokenizer/Config_Linux/\")
# SENTENCE SPLITTER
add_definitions(-DSSPLITTER_CONFIG_RUSSIAN=\"${CONFIG_ROOT_DIR}/SentenceSplitter/RU/sent_split_model\")
add_definitions(-DSSPLITTER_CONFIG_ENGLISH=\"${CONFIG_ROOT_DIR}/SentenceSplitter/EN/sent_split_model\")
# STRESSER
add_definitions(-DSTRESSER_CONFIG_FOLDER=\"${CONFIG_ROOT_DIR}/Stresser/Config_Linux/\")
#SYNTAX
add_definitions(-DSYNTAX_CONFIG_FOLDER=\"${CONFIG_ROOT_DIR}/SyntaxParser/Config_Linux/\")
add_definitions(-DSYNTAX_DRAW_TREE_SCRIPT=\"${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Drawer/drawTree.sh\")
add_definitions(-DSYNTAX_OUTPUT_TREE_PATH=\"${CONFIG_ROOT_DIR}/SyntaxParser/tree.dot\")
#SYNTAX_SUMMARY
add_definitions(-DSYNTAX_SUMMARY_TOKEN_IMPORTANCE_MODEL=\"${CONFIG_ROOT_DIR}/SyntaxSummary/Config_Linux/token-importance-model.txt\")
add_definitions(-DSYNTAX_SUMMARY_TALK_TOKENS_FILE=\"${CONFIG_ROOT_DIR}/SyntaxSummary/Config_Linux/talk-tokens.txt\")
add_definitions(-DSYNTAX_SUMMARY_SRC_FOLDER=\"${PROJECT_ROOT_DIR}/applications/SyntaxSummary/\")
# MORPHOLOGY_ON_SYNTAX
add_definitions(-DMORPHOLOGY_ON_SYNTAX_CONFIG_FOLDER=\"${CONFIG_ROOT_DIR}/MorphologyOnSyntax/Config_Linux/\")
# STAT CALC
add_definitions(-DSTAT_CALC_PREPROC_RULES_EN=\"${CONFIG_ROOT_DIR}/StatCalc/preprocessing_rules_eng.txt\")
add_definitions(-DSTAT_CALC_PREPROC_RULES_RU=\"${CONFIG_ROOT_DIR}/StatCalc/preprocessing_rules.txt\")
#NEWS_READER_DEMO
add_definitions(-DNEWS_READER_DEMO_DATA=\"${SMAUG_PATH}NewsReaderDemo/Data/\")
#NEWS_READER_DEMO_ENG
add_definitions(-DNEWS_READER_DEMO_ENG_DATA=\"${SMAUG_PATH}NewsReaderDemo-eng2/Data/\")
#TEXT_SUMMARIZATION
add_definitions(-DTEXT_SUMMARIZATION_DATA=\"/home/irina/data/TextSummarization/\")
#TEXT_SUMMARIZATION_ENG
add_definitions(-DTEXT_SUMMARIZATION_ENG_DATA=\"${SMAUG_PATH}TextSummarization-eng/\")
#TOOLS
add_definitions(-DTOOLS_OS_SCRIPT_FOLDER=\"${PROJECT_ROOT_DIR}/utils/Tools/OS/Scripts/\")

add_subdirectory(src)
add_subdirectory(test)


