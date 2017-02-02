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

include (../src/common_includes.cmake)

add_subdirectory(google-test google-test)

set(TARGET_NAME unit-test)

add_definitions(-Wl,--whole-archive)

set(unit_test_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(unit_test_INCLUDE_DIR ${PROJECT_INCLUDE_DIR})

set(unit_test_SOURCES
    ${unit_test_SOURCE_DIR}/google-test/src/gtest_main.cc
    ${unit_test_SOURCE_DIR}/test-dictionary.cpp
    ${unit_test_SOURCE_DIR}/test_dict_tokenizer.cpp
)

set(unit_test_HEADERS
)

include_directories(
    ${unit_test_SOURCE_DIR}/google-test
    ${unit_test_SOURCE_DIR}/google-test/gtest
    ${dictionary_subfolders}
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/Tools/Exceptions
)


add_executable(${TARGET_NAME} ${unit_test_HEADERS} ${unit_test_SOURCES})

target_link_libraries(${TARGET_NAME} google-test dictionary) # tools crf dictionary disambiguation regexptools tagsetconstants)

