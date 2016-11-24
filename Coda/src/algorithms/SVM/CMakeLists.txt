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

set(TARGET_NAME svm)
set(svm_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(svm_SOURCES
    ${svm_SOURCE_DIR}/libsvm-wrapper.cpp
    ${svm_SOURCE_DIR}/svm.cpp
)

set(svm_HEADERS
    ${svm_SOURCE_DIR}/libsvm-wrapper.h
    ${svm_SOURCE_DIR}/svm.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/utils/Tools
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${svm_SOURCES} ${svm_HEADERS})
add_executable(${TARGET_NAME}-app ${svm_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
