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

set(TARGET_NAME porterstemmer)
set(porterstemmer_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(porterstemmer_SOURCES
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_danish.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_dutch.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_english.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_finnish.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_french.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_german.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_hungarian.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_italian.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_norwegian.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_porter.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_portuguese.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_romanian.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_russian.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_spanish.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_swedish.c
      ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_turkish.c
      ${porterstemmer_SOURCE_DIR}/runtime/api.c
      ${porterstemmer_SOURCE_DIR}/runtime/utilities.c
      ${porterstemmer_SOURCE_DIR}/libstemmer/libstemmer_utf8.c
)
#MESSAGE("${porterstemmer_SOURCES}")
set(porterstemmer_HEADERS
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_danish.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_dutch.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_english.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_finnish.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_french.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_german.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_hungarian.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_italian.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_norwegian.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_porter.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_portuguese.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_romanian.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_russian.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_spanish.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_swedish.h
    ${porterstemmer_SOURCE_DIR}/src_c/stem_UTF_8_turkish.h
    ${porterstemmer_SOURCE_DIR}/include/libstemmer.h
    ${porterstemmer_SOURCE_DIR}/libstemmer/modules_utf8.h
    ${porterstemmer_SOURCE_DIR}/runtime/api.h
    ${porterstemmer_SOURCE_DIR}/runtime/header.h
)

include_directories(
    ${porterstemmer_SOURCE_DIR}/include/
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core
)

add_library(
    ${TARGET_NAME} ${porterstemmer_SOURCES}
    ${porterstemmer_HEADERS}
    ${porterstemmer_SOURCE_DIR}/stemmer.cpp
    ${porterstemmer_SOURCE_DIR}/stemmer.h
)

add_executable(${TARGET_NAME}-app ${porterstemmer_SOURCE_DIR}/examples/stemwords.cpp)
target_link_libraries(${TARGET_NAME} tools)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
