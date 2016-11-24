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

# src/tagset-constants
# retrieval-app demo application

set(TARGET_NAME tagsetconstants)
set(TAGSET_CONSTANTS_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(TAGSET_CONSTANTS_SOURCES
    ${TAGSET_CONSTANTS_SOURCE_DIR}/tagsetConstants.cpp
)

set(TAGSET_CONSTANTS_HEADERS
    ${TAGSET_CONSTANTS_SOURCE_DIR}/tagsetConstants.h
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${TAGSET_CONSTANTS_SOURCES} ${TAGSET_CONSTANTS_HEADERS})
ADD_DEFINITIONS("-Wall" "-fPIC")
