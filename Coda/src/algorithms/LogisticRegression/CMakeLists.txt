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

set(TARGET_NAME logistic-regression)
set(regression_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(regression_SOURCES
    ${regression_SOURCE_DIR}/softmax-regression.cpp
    ${regression_SOURCE_DIR}/softmax-regression-trainer.cpp
    ${regression_SOURCE_DIR}/features-reader.cpp
    ${regression_SOURCE_DIR}/features-writer.cpp
    ${regression_SOURCE_DIR}/features-filter.cpp
    ${regression_SOURCE_DIR}/sample.cpp
)

set(regression_HEADERS
    ${regression_SOURCE_DIR}/softmax-regression.h
    ${regression_SOURCE_DIR}/softmax-regression-trainer.h
    ${regression_SOURCE_DIR}/features-reader.h
    ${regression_SOURCE_DIR}/features-writer.h
    ${regression_SOURCE_DIR}/features-filter.h
    ${regression_SOURCE_DIR}/sample.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/utils/Tools

    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Clib
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/EventHandler
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${regression_SOURCES} ${regression_HEADERS})
target_link_libraries(${TARGET_NAME} tools optimization)
add_executable(${TARGET_NAME}-app ${regression_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")

