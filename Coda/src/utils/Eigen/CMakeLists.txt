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

# src/utils/Eigen

set(TARGET_NAME eigen)
set(eigen_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(eigen_SOURCES
    ${eigen_SOURCE_DIR}/eigen-tools.cpp

)

set(eigen_HEADERS
    ${eigen_SOURCE_DIR}/eigen.h
    ${eigen_SOURCE_DIR}/eigen-tools.h
    ${eigen_SOURCE_DIR}/eigen-data-types.h
)

set(CMAKE_INCLUDE_SYSTEM_FLAG_${lang} "-isystem ")

include_directories(
    ${eigen_SOURCE_DIR}/Eigen
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${eigen_SOURCES} ${eigen_HEADERS})
SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE C)
#set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL eigen_EXPORTS)
add_executable(${TARGET_NAME}-app ${eigen_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
#ADD_DEFINITIONS("-Wall" "-fPIC")
