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

# src/retrieval-app
# retrieval-app demo application

set(TARGET_NAME stresser)
set(stresser_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(stresser_SOURCES
    ${stresser_SOURCE_DIR}/Core/StresserCreator.cpp
    ${stresser_SOURCE_DIR}/Rus/StresserRus.cpp
)

set(stresser_HEADERS
    ${stresser_SOURCE_DIR}/Core/IStresser.h
    ${stresser_SOURCE_DIR}/Core/StresserCreator.h
    ${stresser_SOURCE_DIR}/Rus/StresserRus.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/algorithms/CRF
    ${PROJECT_ROOT_DIR}/algorithms/Optimization

    ${PROJECT_ROOT_DIR}/nlp-stack/Dictionary
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Disambiguator
    ${PROJECT_ROOT_DIR}/nlp-stack/Stresser/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/Stresser/Rus
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core

    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/FileManager
)


add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${stresser_SOURCES} ${stresser_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL stresser_EXPORTS)
add_executable(${TARGET_NAME}-app ${stresser_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools csvManager tokenizer crf dictionary disambiguator)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
