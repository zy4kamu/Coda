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

include (../../common_includes.cmake)

set(CMAKE_INCLUDE_SYSTEM_FLAG_${lang} "-isystem ")

include_directories(
    SYSTEM ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex
    tools_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}
)

add_library(${project_name} ${LIBRARY_TYPE} ${sources} ${headers})
target_link_libraries(${project_name})

add_executable(${project_name}-app main.cpp)
target_link_libraries(${project_name}-app ${project_name})





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

# src/Tools
# retrieval-app demo application

#set(TARGET_NAME tools)
#set(tools_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

#set(tools_SOURCES
#    ${tools_SOURCE_DIR}/IO/BinaryIO.cpp
#    ${tools_SOURCE_DIR}/IO/ConsoleTools.cpp

#    ${tools_SOURCE_DIR}/Containers/Alphabet.cpp
#    ${tools_SOURCE_DIR}/Containers/LinearHashFunction.cpp
#    ${tools_SOURCE_DIR}/Containers/SparseVector.cpp
#    ${tools_SOURCE_DIR}/Containers/Document.cpp

#    ${tools_SOURCE_DIR}/Locale/Language.cpp
#    ${tools_SOURCE_DIR}/Locale/LocaleSettings.cpp
#    ${tools_SOURCE_DIR}/Locale/RussianLocale.cpp
#    ${tools_SOURCE_DIR}/Locale/ToolsCyrillicSymbols.cpp

#    ${tools_SOURCE_DIR}/OS/OS.cpp
#    ${tools_SOURCE_DIR}/OS/time-calc.cpp
	
#    ${tools_SOURCE_DIR}/Exceptions.cpp
#    ${tools_SOURCE_DIR}/Strings/LinguisticTools.cpp
#    ${tools_SOURCE_DIR}/Strings/StringConvertions.cpp
#    ${tools_SOURCE_DIR}/Strings/StringTools.cpp
#)

#set(tools_HEADERS
#    ${tools_SOURCE_DIR}/IO/BinaryIO.h
#    ${tools_SOURCE_DIR}/IO/ConsoleTools.h

#    ${tools_SOURCE_DIR}/Containers/Alphabet.h
#    ${tools_SOURCE_DIR}/Containers/ContainerConverters.hpp
#    ${tools_SOURCE_DIR}/Containers/ContainerEraser.h
#    ${tools_SOURCE_DIR}/Containers/ContainerGenerators.hpp
#    ${tools_SOURCE_DIR}/Containers/ContainerInserter.h
#    ${tools_SOURCE_DIR}/Containers/ContainerSearcher.hpp
#    ${tools_SOURCE_DIR}/Containers/ContainerTools.h
#    ${tools_SOURCE_DIR}/Containers/Document.h
#    ${tools_SOURCE_DIR}/Containers/FixedHashMap.h
#    ${tools_SOURCE_DIR}/Containers/LinearHashFunction.h
#    ${tools_SOURCE_DIR}/Containers/ContainerSorter.hpp
#    ${tools_SOURCE_DIR}/Containers/SparseVector.h
#    ${tools_SOURCE_DIR}/Containers/Trie.h
#    ${tools_SOURCE_DIR}/Containers/TernarySearchTree.h
#    ${tools_SOURCE_DIR}/Containers/TypeTraits.h

#    ${tools_SOURCE_DIR}/Locale/Language.h
#    ${tools_SOURCE_DIR}/Locale/LocaleSettings.hpp
#    ${tools_SOURCE_DIR}/Locale/RussianLocale.h
#    ${tools_SOURCE_DIR}/Locale/ToolsCyrillicSymbols.h

#    ${tools_SOURCE_DIR}/Math/MatrixTools.h
#    ${tools_SOURCE_DIR}/Math/TableCalculator.hpp

#    ${tools_SOURCE_DIR}/OS/FixedSizeConnecter.hpp
#    ${tools_SOURCE_DIR}/OS/FloatSizeConnecter.hpp
#    ${tools_SOURCE_DIR}/OS/PythonFloatSizeConnecter.hpp
#    ${tools_SOURCE_DIR}/OS/OS.h
#    ${tools_SOURCE_DIR}/OS/time-calc.h
#    ${tools_SOURCE_DIR}/OS/ProcessClient.hpp
#    ${tools_SOURCE_DIR}/OS/ProcessServer.hpp
#    ${tools_SOURCE_DIR}/OS/ProcessInetClient.hpp
#    ${tools_SOURCE_DIR}/OS/ProcessInetServer.hpp

#    ${tools_SOURCE_DIR}/Strings/AhoCorasickParser.hpp
#    ${tools_SOURCE_DIR}/Strings/LinguisticTools.h
#    ${tools_SOURCE_DIR}/Strings/Splitter.h
#    ${tools_SOURCE_DIR}/Strings/StringConvertions.h
#    ${tools_SOURCE_DIR}/Strings/StringTools.h

#    ${tools_SOURCE_DIR}/DataFunctors.h
#    ${tools_SOURCE_DIR}/Exceptions.h
#    ${tools_SOURCE_DIR}/Tools.h
#    ${tools_SOURCE_DIR}/CrossPlatformHeader.h
#)

#set(CMAKE_INCLUDE_SYSTEM_FLAG_${lang} "-isystem ")

#include_directories(
#    SYSTEM ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex
#    ${tools_SOURCE_DIR}/
#)

#add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${tools_SOURCES} ${tools_HEADERS})
#set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL tools_EXPORTS)
#add_executable(${TARGET_NAME}-app ${tools_SOURCE_DIR}/main.cpp)
#target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
#ADD_DEFINITIONS("-Wall" "-fPIC")
