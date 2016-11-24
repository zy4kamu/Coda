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

# src/Dictionary
# retrieval-app demo application

set(TARGET_NAME dictionary)
set(dictionary_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(dictionary_SOURCES
    ${dictionary_SOURCE_DIR}/Dictionary.cpp
    ${dictionary_SOURCE_DIR}/DictionaryNode.cpp
    ${dictionary_SOURCE_DIR}/DictionaryNodeModel.cpp
    ${dictionary_SOURCE_DIR}/DictionaryTools.cpp
    ${dictionary_SOURCE_DIR}/DictionaryTrie.cpp
    ${dictionary_SOURCE_DIR}/DictionaryTrieModel.cpp
    ${dictionary_SOURCE_DIR}/DictionaryTrieModelElement.cpp
    ${dictionary_SOURCE_DIR}/DictionaryTrieBuild.cpp
    ${dictionary_SOURCE_DIR}/DictionaryTrieBinaryFileCreator.cpp
    ${dictionary_SOURCE_DIR}/DictionaryTrieBinaryFileReader.cpp
    ${dictionary_SOURCE_DIR}/DictionaryTrieWordFormByFeatures.cpp
    ${dictionary_SOURCE_DIR}/DictionaryTrieAccent.cpp
    ${dictionary_SOURCE_DIR}/DictionaryTrieAccentUpdate.cpp
    ${dictionary_SOURCE_DIR}/DictionaryTrieRebuild.cpp
    ${dictionary_SOURCE_DIR}/DictionaryTrieIterator.cpp
    ${dictionary_SOURCE_DIR}/MorphologicalDictionary.cpp
    ${dictionary_SOURCE_DIR}/MorphologicalDictionaryBackup.cpp
    ${dictionary_SOURCE_DIR}/NGramNode.cpp
    ${dictionary_SOURCE_DIR}/NGramTrie.cpp
    ${dictionary_SOURCE_DIR}/NGramTrieBuild.cpp
    ${dictionary_SOURCE_DIR}/NGramTrieBinaryFileCreator.cpp
    ${dictionary_SOURCE_DIR}/NGramTrieBinaryFileReader.cpp
    ${dictionary_SOURCE_DIR}/SuffixModelNode.cpp
    ${dictionary_SOURCE_DIR}/SuffixModelTrie.cpp
    ${dictionary_SOURCE_DIR}/SuffixModelTrieBuild.cpp
    ${dictionary_SOURCE_DIR}/SuffixModelTrieBinaryFileCreator.cpp
    ${dictionary_SOURCE_DIR}/SuffixModelTrieBinaryFileReader.cpp
    ${dictionary_SOURCE_DIR}/TestMorphologicalAnalyze.cpp
    ${dictionary_SOURCE_DIR}/TestNGram.cpp
)

set(dictionary_HEADERS
    ${dictionary_SOURCE_DIR}/Dictionary.h
    ${dictionary_SOURCE_DIR}/DictionaryDataTypes.h
    ${dictionary_SOURCE_DIR}/DictionaryInterface.h
    ${dictionary_SOURCE_DIR}/DictionaryNode.h
    ${dictionary_SOURCE_DIR}/DictionaryNodeModel.h
    ${dictionary_SOURCE_DIR}/DictionaryTools.h
    ${dictionary_SOURCE_DIR}/DictionaryTrie.h
    ${dictionary_SOURCE_DIR}/DictionaryTrieModel.h
    ${dictionary_SOURCE_DIR}/DictionaryTrieModelElement.h
    ${dictionary_SOURCE_DIR}/DictionaryTrieBuild.h
    ${dictionary_SOURCE_DIR}/DictionaryTrieBinaryFileCreator.h
    ${dictionary_SOURCE_DIR}/DictionaryTrieBinaryFileReader.h
    ${dictionary_SOURCE_DIR}/DictionaryTrieWordFormByFeatures.h
    ${dictionary_SOURCE_DIR}/DictionaryTrieAccent.h
    ${dictionary_SOURCE_DIR}/DictionaryTrieAccentUpdate.h
    ${dictionary_SOURCE_DIR}/DictionaryTrieRebuild.h
    ${dictionary_SOURCE_DIR}/DictionaryTrieIterator.h
    ${dictionary_SOURCE_DIR}/MorphologicalDictionary.h
    ${dictionary_SOURCE_DIR}/MorphologicalDictionaryBackup.h
    ${dictionary_SOURCE_DIR}/NGramNode.h
    ${dictionary_SOURCE_DIR}/NGramTrie.h
    ${dictionary_SOURCE_DIR}/NGramTrieBuild.h
    ${dictionary_SOURCE_DIR}/NGramTrieBinaryFileCreator.h
    ${dictionary_SOURCE_DIR}/NGramTrieBinaryFileReader.h
    ${dictionary_SOURCE_DIR}/SuffixModelNode.h
    ${dictionary_SOURCE_DIR}/SuffixModelTrie.h
    ${dictionary_SOURCE_DIR}/SuffixModelTrieBuild.h
    ${dictionary_SOURCE_DIR}/SuffixModelTrieBinaryFileCreator.h
    ${dictionary_SOURCE_DIR}/SuffixModelTrieBinaryFileReader.h
    ${dictionary_SOURCE_DIR}/TestMorphologicalAnalyze.h
    ${dictionary_SOURCE_DIR}/TestNGram.h
    ${dictionary_SOURCE_DIR}/TestTools.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/utils/Tools
)


add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${dictionary_SOURCES} ${dictionary_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL dictionary_EXPORTS)
add_executable(${TARGET_NAME}-app ${dictionary_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools crf)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
