# Atlas 3D reconstruction
#
# Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
#
# Authors: Andrey Moiseenko <moiseenko.a@samsung.com>,
#          Alexander Romanenko <Alexander Romanenko>, 
#          Irina Piontkovskaya <p.irina@samsung.com>, 
#          Manh Ha Le <mh.le@samsung.com>,
#          Mikhail Kudinov <m.kudinov@samsung.com>, 
#          Stepan Muzychka <s.muzychka@samsung.com>  

# tests/gentest
# general test application

add_subdirectory(google-test google-test)

set(TARGET_NAME unit-test)

add_definitions(-Wl,--whole-archive)

set(unit_test_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(unit_test_INCLUDE_DIR ${PROJECT_INCLUDE_DIR})

set(unit_test_SOURCES
    ${unit_test_SOURCE_DIR}/test-example.cpp
#    ${unit_test_SOURCE_DIR}/test-disambiguator.cpp
    ${unit_test_SOURCE_DIR}/google-test/src/gtest_main.cc
#    ${unit_test_SOURCE_DIR}/test-inflector.cpp
#    ${unit_test_SOURCE_DIR}/test-formatted-io.cpp
#    ${unit_test_SOURCE_DIR}/test-sentence-splitter.cpp
)

set(unit_test_HEADERS
)

include_directories(
    ${unit_test_SOURCE_DIR}/google-test
    ${unit_test_SOURCE_DIR}/google-test/gtest
#    ${PROJECT_ROOT_DIR}/Inflector
#    ${PROJECT_ROOT_DIR}/Tokenizer
#    ${PROJECT_ROOT_DIR}/Tools
#    ${PROJECT_ROOT_DIR}/CSVManager
#    ${PROJECT_ROOT_DIR}/CRF
#    ${PROJECT_ROOT_DIR}/Dictionary
#    ${PROJECT_ROOT_DIR}/Disambiguator
#    ${PROJECT_ROOT_DIR}/TagsetConstants
#    ${PROJECT_ROOT_DIR}/FormattedIO
#    ${PROJECT_ROOT_DIR}/SentenceSplitter/Core
#    ${PROJECT_ROOT_DIR}/Optimization
#    ${PROJECT_ROOT_DIR}/RegexpTools/boost_regex/
#    ${PROJECT_ROOT_DIR}/LogisticRegression
#    ${PROJECT_ROOT_DIR}/TagsetConverter
)

#if (MSVC)
#   add_definitions(-DDIS_CONFIG_PATH="${PROJECT_ROOT_DIR}/Disambiguation/Config_Windows/")
#   add_definitions(-DINFLECTOR_CONFIG_PATH="${PROJECT_ROOT_DIR}/Inflector/Config_Windows/")
#elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
#    add_definitions(-DDIS_CONFIG_PATH=\"${PROJECT_ROOT_DIR}/Disambiguation/Config_Linux/\")
#    add_definitions(-DINFLECTOR_CONFIG_PATH=\"${PROJECT_ROOT_DIR}/Inflector/Config_Linux/\")
#endif()


add_definitions(-DTOKENSTREAM_IO_TEST_FILE="${unit_test_SOURCE_DIR}/token_stream_test_file")

add_executable(${TARGET_NAME} ${unit_test_HEADERS} ${unit_test_SOURCES})

target_link_libraries(${TARGET_NAME} google-test) # tools crf dictionary disambiguation regexptools tagsetconstants)

