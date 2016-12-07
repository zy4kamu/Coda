# Object Retrieval Engine
#
# Copyright (c) 2013 Samsung Electronics Co., Ltd. All rights reserved.
#
# Authors: Anton Sorokin <anton.s@samsung.com>

# src/sql-io/sqlite

set(TARGET_NAME sqlite3)
set(sqlite3_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})


if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpic")
endif()

add_library(${TARGET_NAME} ${LIBRARY_TYPE} 
    ${sqlite3_SOURCE_DIR}/sqlite3.c
    ${sqlite3_SOURCE_DIR}/sqlite3.def
    ${sqlite3_SOURCE_DIR}/sqlite3.h
)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(CUSTOM_PTHREAD_LIBS ${PROJECT_SOURCE_DIR}/pthread/lib)
    set(CUSTOM_PTHREAD_DLL ${PROJECT_SOURCE_DIR}/pthread/dll)
    if (${bitness} EQUAL 32)
        set(CUSTOM_PTHREAD_LIBS ${CUSTOM_PTHREAD_LIBS}/x86)
        set(CUSTOM_PTHREAD_DLL ${CUSTOM_PTHREAD_DLL}/x86)
    else()
        set(CUSTOM_PTHREAD_LIBS ${CUSTOM_PTHREAD_LIBS}/x64)
        set(CUSTOM_PTHREAD_DLL ${CUSTOM_PTHREAD_DLL}/x64)
    endif()
    if (MSVC)
        set(CUSTOM_PTHREAD_LIBS ${CUSTOM_PTHREAD_LIBS}/pthreadVC2.lib)
        file(COPY ${CUSTOM_PTHREAD_DLL}/pthreadVC2.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH})
    else()
        set(CUSTOM_PTHREAD_LIBS ${CUSTOM_PTHREAD_LIBS}/pthreadGC2.lib)
        file(COPY ${CUSTOM_PTHREAD_DLL}/pthreadGC2.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH})
    endif()
	target_link_libraries(${TARGET_NAME} ${CUSTOM_PTHREAD_LIBS})
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        target_link_libraries(${TARGET_NAME} pthread dl)
endif()




