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

# src/FileManager
# retrieval-app demo application

set(TARGET_NAME file-manager)
set(file-manager_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(file-manager_SOURCES
	${file-manager_SOURCE_DIR}/FileReader.cpp
    ${file-manager_SOURCE_DIR}/main.cpp
)

set(file-manager_HEADERS
	${file-manager_SOURCE_DIR}/BinaryReader.hpp
	${file-manager_SOURCE_DIR}/BinaryWriter.hpp
	${file-manager_SOURCE_DIR}/FileStreamIdentifier.h
	${file-manager_SOURCE_DIR}/ReadConverter.h
	${file-manager_SOURCE_DIR}/FileReader.h
	${file-manager_SOURCE_DIR}/FileWriter.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/utils/Tools
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${file-manager_SOURCES} ${file-manager_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL file-manager_EXPORTS)
add_executable(${TARGET_NAME}-app ${file-manager_SOURCES} ${file-manager_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
