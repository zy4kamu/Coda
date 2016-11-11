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

# src/FileManager
# retrieval-app demo application

set(TARGET_NAME csvManager)
set(csvManager_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(csvManager_SOURCES
	${csvManager_SOURCE_DIR}/FileReader.cpp
    ${csvManager_SOURCE_DIR}/main.cpp
)

set(csvManager_HEADERS
	${csvManager_SOURCE_DIR}/BinaryReader.hpp
	${csvManager_SOURCE_DIR}/BinaryWriter.hpp
	${csvManager_SOURCE_DIR}/FileStreamIdentifier.h
	${csvManager_SOURCE_DIR}/ReadConverter.h
	${csvManager_SOURCE_DIR}/FileReader.h
	${csvManager_SOURCE_DIR}/FileWriter.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/utils/Tools
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${csvManager_SOURCES} ${csvManager_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL csvManager_EXPORTS)
add_executable(${TARGET_NAME}-app ${csvManager_SOURCES} ${csvManager_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
