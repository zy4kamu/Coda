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

get_filename_component(project_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" project_name ${project_name})
string(TOLOWER ${project_name} project_name)

file(GLOB sources "*.cpp" "*/*.cpp" "*/*/*.cpp")
file(GLOB headers "*.h" "*/*.h" "*/*/*.h")
file(GLOB sourceheaders "*.hpp" "*/*.hpp" "*/*/*.hpp")
file(GLOB crf_subfolders ${PROJECT_ROOT_DIR}/algorithms/CRF/*/)

include_directories(
    ${crf_subfolders}

    ${PROJECT_ROOT_DIR}/utils/FileManager
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/Tools/Strings

    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Clib
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/EventHandler
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer
)

ADD_DEFINITIONS("-Wall" "-fPIC")

add_library(${project_name} ${LIBRARY_TYPE} ${sources} ${headers})
target_link_libraries(${project_name} tools optimization csvManager gomp)

add_executable(${project_name}-app main.cpp)
target_link_libraries(${project_name}-app ${project_name})
