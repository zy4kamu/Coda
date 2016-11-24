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

# src/optimization
# retrieval-app demo application

set(TARGET_NAME optimization)
set(optimization_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(optimization_SOURCES
    ${optimization_SOURCE_DIR}/Clib/c_lbfgs.c

    ${optimization_SOURCE_DIR}/Optimizer/IOptimizable.cpp
    ${optimization_SOURCE_DIR}/Optimizer/LBFGS.cpp
)

set(optimization_HEADERS
    ${optimization_SOURCE_DIR}/Clib/arithmetic_ansi.h
    ${optimization_SOURCE_DIR}/Clib/arithmetic_sse_double.h
    ${optimization_SOURCE_DIR}/Clib/arithmetic_sse_float.h
    ${optimization_SOURCE_DIR}/Clib/c_lbfgs.h

    ${optimization_SOURCE_DIR}/EventHandler/ICallable.h

    ${optimization_SOURCE_DIR}/Optimizer/IOptimizable.h
    ${optimization_SOURCE_DIR}/Optimizer/LBFGS.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Clib
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/EventHandler
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer
    ${PROJECT_ROOT_DIR}/utils/Tools
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${optimization_SOURCES} ${optimization_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL optimization_EXPORTS)
add_executable(${TARGET_NAME}-app ${optimization_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools crf dictionary)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
