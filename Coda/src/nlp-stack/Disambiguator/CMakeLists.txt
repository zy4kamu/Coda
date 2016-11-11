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

set(TARGET_NAME disambiguator)
set(disambiguator_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(disambiguator_SOURCES
    ${disambiguator_SOURCE_DIR}/Core/Creator/DisambiguatorCreator.cpp

    ${disambiguator_SOURCE_DIR}/Core/Controller/Controller.cpp

    ${disambiguator_SOURCE_DIR}/Core/DataPreparer/TrainingAndTestDataPreparer.cpp

    ${disambiguator_SOURCE_DIR}/Core/Disambiguator/Disambiguator.cpp

    ${disambiguator_SOURCE_DIR}/Core/Exceptions/LanguageNotRegisteredException.cpp

    ${disambiguator_SOURCE_DIR}/Core/ReLearner/DisambiguatorRelearner.cpp

    ${disambiguator_SOURCE_DIR}/Core/Settings/DefaultDisambiguatorSettings.cpp
    ${disambiguator_SOURCE_DIR}/Core/Settings/DisambiguatorSettingsHolder.cpp

    ${disambiguator_SOURCE_DIR}/Core/Tester/DisambiguatorTester.cpp

    ${disambiguator_SOURCE_DIR}/Rus/ChainTransformer/FullMorphologyChainTransformer.cpp

    ${disambiguator_SOURCE_DIR}/Rus/Disambiguator/DisambiguatorRus.cpp

    ${disambiguator_SOURCE_DIR}/Rus/DataPreparer/ConfigPreparerRus.cpp

    ${disambiguator_SOURCE_DIR}/Rus/FeatureCalculator/FeatureCalculatorRus.cpp
    ${disambiguator_SOURCE_DIR}/Rus/FeatureCalculator/NameEntityFeatureCalculatorRus.cpp
    ${disambiguator_SOURCE_DIR}/Rus/FeatureCalculator/SchemaFeatureCalculatorRus.cpp
    ${disambiguator_SOURCE_DIR}/Rus/FeatureCalculator/SimpleFeatureCalculatorRus.cpp

    ${disambiguator_SOURCE_DIR}/Rus/PossibleStateFinder/DoublingPossibleStateFinderRus.cpp
    ${disambiguator_SOURCE_DIR}/Rus/PossibleStateFinder/FullMorphologyPossibleStateFinder.cpp

    ${disambiguator_SOURCE_DIR}/Rus/Settings/DisambiguatorSettingsRus.cpp

    ${disambiguator_SOURCE_DIR}/En/FeatureCalculator/FeatureCalculatorEn.cpp
    ${disambiguator_SOURCE_DIR}/En/Settings/DisambiguatorSettingsEn.cpp

    ${disambiguator_SOURCE_DIR}/EnFast/FeatureCalculator/FeatureCalculatorEnFast.cpp
    ${disambiguator_SOURCE_DIR}/EnFast/Settings/DisambiguatorSettingsEnFast.cpp
)

set(disambiguator_HEADERS
    ${disambiguator_SOURCE_DIR}/Core/Creator/DisambiguatorCreator.h

    ${disambiguator_SOURCE_DIR}/Core/Controller/Controller.h

    ${disambiguator_SOURCE_DIR}/Core/DataPreparer/TrainingAndTestDataPreparer.h

    ${disambiguator_SOURCE_DIR}/Core/Disambiguator/Disambiguator.h
    ${disambiguator_SOURCE_DIR}/Core/Disambiguator/DisambiguatedData.h
    ${disambiguator_SOURCE_DIR}/Core/Disambiguator/IDisambiguator.h
    ${disambiguator_SOURCE_DIR}/Core/Disambiguator/PreDisambiguatedData.h

    ${disambiguator_SOURCE_DIR}/Core/Exceptions/LanguageNotRegisteredException.h

    ${disambiguator_SOURCE_DIR}/Core/FeatureCalculator/IFeatureCalculator.h

    ${disambiguator_SOURCE_DIR}/Core/ReLearner/DisambiguatorRelearner.h

    ${disambiguator_SOURCE_DIR}/Core/Settings/DefaultDisambiguatorSettings.h
    ${disambiguator_SOURCE_DIR}/Core/Settings/DisambiguatorSettingsHolder.h
    ${disambiguator_SOURCE_DIR}/Core/Settings/IDisambiguatorSettings.h

    ${disambiguator_SOURCE_DIR}/Core/Tester/DisambiguatorTester.h

    ${disambiguator_SOURCE_DIR}/Rus/ChainTransformer/FullMorphologyChainTransformer.h

    ${disambiguator_SOURCE_DIR}/Rus/Disambiguator/DisambiguatorRus.h

    ${disambiguator_SOURCE_DIR}/Rus/DataPreparer/ConfigPreparerRus.h

    ${disambiguator_SOURCE_DIR}/Rus/FeatureCalculator/FeatureCalculatorRus.h

    ${disambiguator_SOURCE_DIR}/Rus/PossibleStateFinder/DoublingPossibleStateFinderRus.h
    ${disambiguator_SOURCE_DIR}/Rus/PossibleStateFinder/FullMorphologyPossibleStateFinder.h

    ${disambiguator_SOURCE_DIR}/Rus/Settings/DisambiguatorSettingsRus.h

    ${disambiguator_SOURCE_DIR}/Rus/FeatureCalculator/NameEntityFeatureCalculatorRus.h
    ${disambiguator_SOURCE_DIR}/Rus/FeatureCalculator/SchemaFeatureCalculatorRus.h
    ${disambiguator_SOURCE_DIR}/Rus/FeatureCalculator/SimpleFeatureCalculatorRus.h

    ${disambiguator_SOURCE_DIR}/En/FeatureCalculator/FeatureCalculatorEn.h
    ${disambiguator_SOURCE_DIR}/En/Settings/DisambiguatorSettingsEn.h

    ${disambiguator_SOURCE_DIR}/EnFast/FeatureCalculator/FeatureCalculatorEnFast.h
    ${disambiguator_SOURCE_DIR}/EnFast/Settings/DisambiguatorSettingsEnFast.h
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

        ${PROJECT_ROOT_DIR}/algorithms/Optimization/Clib
        ${PROJECT_ROOT_DIR}/algorithms/Optimization/EventHandler
        ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer

        ${PROJECT_ROOT_DIR}/utils/FileManager
        ${PROJECT_ROOT_DIR}/utils/TagsetConverter
        ${PROJECT_ROOT_DIR}/utils/Tools

        ${PROJECT_ROOT_DIR}/nlp-stack/Dictionary
        ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core
        ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Controller
        ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Creator
        ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/DataPreparer
        ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Disambiguator
        ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Exceptions
        ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/FeatureCalculator
        ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/ReLearner
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
        ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/EnFast/FeatureCalculator
        ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/EnFast/Settings
        ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core

)

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${disambiguator_SOURCES} ${disambiguator_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL disambiguator_EXPORTS)
add_executable(${TARGET_NAME}-app ${disambiguator_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools csvManager optimization crf dictionary tokenizer tagsetConverter )
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
