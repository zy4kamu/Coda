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

set(TARGET_NAME tokenizer)
set(tokenizer_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(tokenizer_SOURCES
    ${tokenizer_SOURCE_DIR}/main.cpp
    ${tokenizer_SOURCE_DIR}/Core/TokenizerCreator.cpp
    ${tokenizer_SOURCE_DIR}/Core/Exceptions/LanguageNotRegisteredException.cpp
    ${tokenizer_SOURCE_DIR}/Rus/TokenizerRus.cpp
    ${tokenizer_SOURCE_DIR}/En/TokenizerEn.cpp
    ${tokenizer_SOURCE_DIR}/EnFast/TokenizerEnFast.cpp
)

set(tokenizer_HEADERS
    ${tokenizer_SOURCE_DIR}/Core/ITokenizer.h
    ${tokenizer_SOURCE_DIR}/Core/Token.h
    ${tokenizer_SOURCE_DIR}/Core/TokenizerCreator.h
    ${tokenizer_SOURCE_DIR}/Core/Exceptions/LanguageNotRegisteredException.h
    ${tokenizer_SOURCE_DIR}/Rus/TokenizerRus.h
    ${tokenizer_SOURCE_DIR}/En/TokenizerEn.h
    ${tokenizer_SOURCE_DIR}/EnFast/TokenizerEnFast.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core/Exceptions
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/En
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/EnFast
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Rus
    ${PROJECT_ROOT_DIR}/utils/FileManager
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex/
    ${PROJECT_ROOT_DIR}/utils/RegexpTools/
)


add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${tokenizer_SOURCES} ${tokenizer_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL tokenizer_EXPORTS)
add_executable(${TARGET_NAME}-app ${tokenizer_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools regexptools csvManager)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
