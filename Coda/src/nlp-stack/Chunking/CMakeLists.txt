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

include (../../common_includes.cmake)

include_directories(
    ${crf_subfolders}

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

add_library(${project_name} ${LIBRARY_TYPE} ${sources} ${headers})
target_link_libraries(${project_name} tools tokenizer tagsetConverter file-manager crf disambiguator tagsetconstants dictionary)

add_executable(${project_name}-app main.cpp)
target_link_libraries(${project_name}-app ${project_name})

