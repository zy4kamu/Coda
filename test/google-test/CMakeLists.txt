# Atlas 3D reconstruction
#
# Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
#
# Authors: Victor Bucha <v.bucha@samsung.com>, Vitaly Chernov <v.chernov@samsung.com>,
#          Aleksei Cherniavskii <aleksei.c@samsung.com>, Artem Shamsuarov <a.shamsuarov@samsung.com>,
#          Maxim Alov <m.alov@samsung.com>, Oleg Muratov <o.muratov@samsung.com>, 
#          Sergey Zagoruyko <s.zagoruyko@samsung.com>, Vitaly Gnatyuk <v.gnatyuk@samsung.com>,
#	   Maria Lyubimtseva <l.maria@partner.samsung.com> 

# tests/google
# google testing library

set(TARGET_NAME google-test)

add_definitions(-Wl,--whole-archive)

set(google_test_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(google_test_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/gtest)

include_directories(
    ${google_test_SOURCE_DIR}
    ${dictionary_subfolders}
)

set(google_test_SOURCES
    ${google_test_SOURCE_DIR}/gtest-all.cc
)

set(google_test_HEADERS
    ${google_test_INCLUDE_DIR}/gtest.h
    ${google_test_INCLUDE_DIR}/gtest-death-test.h
    ${google_test_INCLUDE_DIR}/gtest-message.h
    ${google_test_INCLUDE_DIR}/gtest-param-test.h
    ${google_test_INCLUDE_DIR}/gtest-param-test.h.pump
    ${google_test_INCLUDE_DIR}/gtest_pred_impl.h
    ${google_test_INCLUDE_DIR}/gtest-printers.h
    ${google_test_INCLUDE_DIR}/gtest_prod.h
    ${google_test_INCLUDE_DIR}/gtest-spi.h
    ${google_test_INCLUDE_DIR}/gtest-test-part.h
    ${google_test_INCLUDE_DIR}/gtest-typed-test.h
    ${google_test_INCLUDE_DIR}/internal/gtest-death-test-internal.h
    ${google_test_INCLUDE_DIR}/internal/gtest-filepath.h
    ${google_test_INCLUDE_DIR}/internal/gtest-internal.h
    ${google_test_INCLUDE_DIR}/internal/gtest-linked_ptr.h
    ${google_test_INCLUDE_DIR}/internal/gtest-param-util-generated.h
    ${google_test_INCLUDE_DIR}/internal/gtest-param-util-generated.h.pump
    ${google_test_INCLUDE_DIR}/internal/gtest-param-util.h
    ${google_test_INCLUDE_DIR}/internal/gtest-port.h
    ${google_test_INCLUDE_DIR}/internal/gtest-string.h
    ${google_test_INCLUDE_DIR}/internal/gtest-tuple.h
    ${google_test_INCLUDE_DIR}/internal/gtest-tuple.h.pump
    ${google_test_INCLUDE_DIR}/internal/gtest-type-util.h
    ${google_test_INCLUDE_DIR}/internal/gtest-type-util.h.pump
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${google_test_HEADERS} ${google_test_SOURCES})

if (NOT(${BUILD_FOR_ANDROID}))
    target_link_libraries(${TARGET_NAME} pthread)
endif()
