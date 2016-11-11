# Object Cadenza
#
# Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
#
# Authors: Andrey Moiseenko <moiseenko.a@samsung.com>,
#          Alexander Romanenko <Alexander Romanenko>, 
#          Irina Piontkovskaya <p.irina@samsung.com>, 
#          Manh Ha Le <mh.le@samsung.com>,
#          Mikhail Kudinov <m.kudinov@samsung.com>, 
#          Stepan Muzychka <s.muzychka@samsung.com>  

# src/retrieval-app
# retrieval-app demo application

set(TARGET_NAME statcalc)
set(statcalc_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(statcalc_SOURCES
    ${statcalc_SOURCE_DIR}/stat-calc.cpp
    ${statcalc_SOURCE_DIR}/document-processor.cpp
    ${statcalc_SOURCE_DIR}/stat-calc-tools.cpp
    ${statcalc_SOURCE_DIR}/statcalc-enums.cpp
)

set(statcalc_HEADERS
    ${statcalc_SOURCE_DIR}/statcalc-data-types.h
    ${statcalc_SOURCE_DIR}/statcalc-enums.h
    ${statcalc_SOURCE_DIR}/stat-calc.h
    ${statcalc_SOURCE_DIR}/stat-calc-tools.h
    ${statcalc_SOURCE_DIR}/document-processor.h
    ${statcalc_SOURCE_DIR}/category-processor.h
#    ${statcalc_SOURCE_DIR}/lemmatizer-adapter.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/algorithms/CRF
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Chain
    ${PROJECT_ROOT_DIR}/algorithms/CRF/ChainTransformer
    ${PROJECT_ROOT_DIR}/algorithms/CRF/IO
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Inference
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Model
    ${PROJECT_ROOT_DIR}/algorithms/CRF/PossibleStateFinder
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Learner
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Settings
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Tester
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Regularization
    ${PROJECT_ROOT_DIR}/algorithms/CRF/ViterbiNode

    ${PROJECT_ROOT_DIR}/nlp-stack/Dictionary
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Controller
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Creator
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/DataPreparer
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Disambiguator
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Exceptions
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/FeatureCalculator
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Learner
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Settings
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Tester
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Rus/ChainTransformer
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Rus/Disambiguator
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Rus/DataPreparer
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Rus/FeatureCalculator
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Rus/PossibleStateFinder
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Rus/Settings
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/En/FeatureCalculator
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/En/Settings
    ${PROJECT_ROOT_DIR}/utils/Eigen

    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Clib
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/EventHandler
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer

#    ${PROJECT_ROOT_DIR}/FormattedIO
    ${PROJECT_ROOT_DIR}/nlp-stack/Lemmatizer
    ${PROJECT_ROOT_DIR}/utils/RegexpTools
    ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex/
    ${PROJECT_ROOT_DIR}/nlp-stack/SentenceSplitter/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/FileManager
    ${PROJECT_ROOT_DIR}/utils/TagsetConverter
#    ${PROJECT_ROOT_DIR}/nlp-stack/TopicModeling
    ${PROJECT_ROOT_DIR}/nlp-stack/PorterStemmer
    ${PROJECT_ROOT_DIR}/nlp-stack/PorterStemmer/include/
)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${statcalc_SOURCES} ${statcalc_HEADERS})
add_executable(${TARGET_NAME}-app ${statcalc_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools dictionary lemmatizer tokenizer disambiguator regexptools sentence-splitter porterstemmer)
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
