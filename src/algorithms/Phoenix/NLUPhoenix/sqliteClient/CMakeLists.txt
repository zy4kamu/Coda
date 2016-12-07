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

# src/sqliteClient
# retrieval-app demo application

add_subdirectory(sqlite sqlite)

set(TARGET_NAME sqlite-client)
set(sqlite-client_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(sqlite-client_SOURCES
    ${sqlite-client_SOURCE_DIR}/serializer.cpp
    ${sqlite-client_SOURCE_DIR}/sqlite_helper.cpp
    ${sqlite-client_SOURCE_DIR}/sqlite_string_processor_base.cpp
)

set(sqlite-client_HEADERS
    ${sqlite-client_SOURCE_DIR}/blob.h
    ${sqlite-client_SOURCE_DIR}/data_base_schema.h
    ${sqlite-client_SOURCE_DIR}/make_queries.h
    ${sqlite-client_SOURCE_DIR}/serializer.h
    ${sqlite-client_SOURCE_DIR}/sqlite3_wrapper.h
    ${sqlite-client_SOURCE_DIR}/sqlite_exceptions.h
    ${sqlite-client_SOURCE_DIR}/sqlite_helper.h
    ${sqlite-client_SOURCE_DIR}/sqlite_string_processor_base.h
    ${sqlite-client_SOURCE_DIR}/sql_table.h
    ${sqlite-client_SOURCE_DIR}/sql_table_base.h
    ${sqlite-client_SOURCE_DIR}/loki/EmptyType.h
    ${sqlite-client_SOURCE_DIR}/loki/HierarchyGenerators.h
    ${sqlite-client_SOURCE_DIR}/loki/NullType.h
    ${sqlite-client_SOURCE_DIR}/loki/Sequence.h
    ${sqlite-client_SOURCE_DIR}/loki/Typelist.h
    ${sqlite-client_SOURCE_DIR}/loki/TypelistMacros.h
    ${sqlite-client_SOURCE_DIR}/loki/TypeManip.h
    ${sqlite-client_SOURCE_DIR}/loki/TypeTraits.h
)

include_directories(
    ${sqlite-client_SOURCE_DIR}/sqlite 
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${sqlite-client_SOURCES} ${sqlite-client_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL sqlite-client_EXPORTS)
target_link_libraries(${TARGET_NAME} sqlite3)
ADD_DEFINITIONS("-Wall" "-fPIC")
