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

# src/retrieval-app
# retrieval-app demo application

set(TARGET_NAME tagsetConverter)
set(tagsetConverter_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(tagsetConverter_SOURCES
    ${tagsetConverter_SOURCE_DIR}/main.cpp
    ${tagsetConverter_SOURCE_DIR}/OpenCorporaToSynagrusConverter.cpp
    ${tagsetConverter_SOURCE_DIR}/RusCorporaToSyntagrusConverter.cpp
    ${tagsetConverter_SOURCE_DIR}/SyntagrusToRusCorporaConverter.cpp
)

set(tagsetConverter_HEADERS
    ${tagsetConverter_SOURCE_DIR}/OpenCorporaToSynagrusConverter.h
    ${tagsetConverter_SOURCE_DIR}/RusCorporaToSyntagrusConverter.h
    ${tagsetConverter_SOURCE_DIR}/SyntagrusToRusCorporaConverter.h
    ${tagsetConverter_SOURCE_DIR}/TagsetConverterInterface.h
)

if (MSVC)
    add_definitions(-DCONFIG_PATH="${PROJECT_ROOT_DIR}/utils/TagsetConverter/Config_Windows/")
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-DCONFIG_PATH=\"${PROJECT_ROOT_DIR}/utils/TagsetConverter/Config_Linux/\")
endif()

include_directories(
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/FileManager
)


add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${tagsetConverter_SOURCES} ${tagsetConverter_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL tagsetConverter_EXPORTS)
add_executable(${TARGET_NAME}-app ${tagsetConverter_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools csvManager)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
