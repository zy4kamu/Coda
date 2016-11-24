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

# src/retrieval-app
# retrieval-app demo application

set(TARGET_NAME syntaxParser)
set(syntaxParser_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(syntaxParser_SOURCES
    ${syntaxParser_SOURCE_DIR}/Core/Batches/ContinuousFeatureBatch.cpp
    ${syntaxParser_SOURCE_DIR}/Core/Batches/DiscreteFeatureBatch.cpp

    ${syntaxParser_SOURCE_DIR}/Core/Creator/SyntaxCreator.cpp

    ${syntaxParser_SOURCE_DIR}/Core/DataPreparer/SyntaxDataPreparer.cpp

    ${syntaxParser_SOURCE_DIR}/Core/Drawer/Drawer.cpp

    ${syntaxParser_SOURCE_DIR}/Core/Exceptions/LanguageNotRegisteredException.cpp
    ${syntaxParser_SOURCE_DIR}/Core/Exceptions/UblasException.cpp

    ${syntaxParser_SOURCE_DIR}/Core/Model/SyntaxModel.cpp

    ${syntaxParser_SOURCE_DIR}/Core/Normalization/MatrixTreeNormalization.cpp

    ${syntaxParser_SOURCE_DIR}/Core/ParseAlgorithms/ChuLiuEdmondDependencySearcher.cpp
    ${syntaxParser_SOURCE_DIR}/Core/ParseAlgorithms/EisnerDependencySearcher.cpp
    ${syntaxParser_SOURCE_DIR}/Core/ParseAlgorithms/GreedyDependencySearcher.cpp

    ${syntaxParser_SOURCE_DIR}/Core/Tester/PerfectMorphologyTreeRecoverer.cpp
    ${syntaxParser_SOURCE_DIR}/Core/Tester/PredictedMorphologyTreeRecoverer.cpp

    ${syntaxParser_SOURCE_DIR}/Core/Tree/SyntaxTree.cpp

    ${syntaxParser_SOURCE_DIR}/En/EdgeCalculator/NodeFeatureCalculatorEn.cpp

    ${syntaxParser_SOURCE_DIR}/En/Settings/SyntaxSettingsEn.cpp

    ${syntaxParser_SOURCE_DIR}/Rus/DataPreparer/SyntaxDataPreparerRus.cpp
    ${syntaxParser_SOURCE_DIR}/Rus/EdgeCalculator/NodeFeatureCalculatorRus.cpp
    ${syntaxParser_SOURCE_DIR}/Rus/EdgeCalculator/SchemaCalculatorRus.cpp
    ${syntaxParser_SOURCE_DIR}/Rus/Settings/SyntaxSettingsRus.cpp

    ${syntaxParser_SOURCE_DIR}/main.cpp
)

set(syntaxParser_HEADERS
    ${syntaxParser_SOURCE_DIR}/Core/Batches/ContinuousFeatureBatch.hpp
    ${syntaxParser_SOURCE_DIR}/Core/Batches/DiscreteFeatureBatch.hpp

    ${syntaxParser_SOURCE_DIR}/Core/Controller/Controller.hpp

    ${syntaxParser_SOURCE_DIR}/Core/Creator/SyntaxCreator.h

    ${syntaxParser_SOURCE_DIR}/Core/DataPreparer/SyntaxDataPreparer.h

    ${syntaxParser_SOURCE_DIR}/Core/Drawer/Drawer.h

    ${syntaxParser_SOURCE_DIR}/Core/Exceptions/LanguageNotRegisteredException.h
    ${syntaxParser_SOURCE_DIR}/Core/Exceptions/UblasException.h

    ${syntaxParser_SOURCE_DIR}/Core/Interfaces/EmptyDataPreparer.h
    ${syntaxParser_SOURCE_DIR}/Core/Interfaces/EmptyEdgeFeatureCalculator.h
    ${syntaxParser_SOURCE_DIR}/Core/Interfaces/EmptyRootFeatureCalculator.h

    ${syntaxParser_SOURCE_DIR}/Core/Learner/SyntaxLearner.hpp

    ${syntaxParser_SOURCE_DIR}/Core/Model/SyntaxModel.h

    ${syntaxParser_SOURCE_DIR}/Core/Normalization/MatrixTreeNormalization.h

    ${syntaxParser_SOURCE_DIR}/Core/ParseAlgorithms/ChuLiuEdmondDependencySearcher.h
    ${syntaxParser_SOURCE_DIR}/Core/ParseAlgorithms/EisnerDependencySearcher.h
    ${syntaxParser_SOURCE_DIR}/Core/ParseAlgorithms/GreedyDependencySearcher.h

    ${syntaxParser_SOURCE_DIR}/Core/Parser/ISyntaxParser.h
    ${syntaxParser_SOURCE_DIR}/Core/Parser/SyntaxParser.hpp
    ${syntaxParser_SOURCE_DIR}/Core/Parser/ConnectionBuilder.hpp

    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Templates/BatchTypeIdentifier.h
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Templates/SyntaxSettings.h
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Templates/TreeTypeIdentifier.h

    ${syntaxParser_SOURCE_DIR}/Core/Tester/PerfectMorphologyTreeRecoverer.h
    ${syntaxParser_SOURCE_DIR}/Core/Tester/PredictedMorphologyTreeRecoverer.h
    ${syntaxParser_SOURCE_DIR}/Core/Tester/SyntaxTester.hpp

    ${syntaxParser_SOURCE_DIR}/Core/Tree/SyntaxNode.h
    ${syntaxParser_SOURCE_DIR}/Core/Tree/SyntaxTree.h

    ${syntaxParser_SOURCE_DIR}/Core/TreeOptimizable/FunctionalSyntaxTreeOptimizable.hpp
    ${syntaxParser_SOURCE_DIR}/Core/TreeOptimizable/SimpleSyntaxTreeOptimizable.hpp
    ${syntaxParser_SOURCE_DIR}/Core/TreeOptimizable/MatrixSyntaxTreeOptimizable.hpp
    ${syntaxParser_SOURCE_DIR}/Core/TreeOptimizable/TreeOptimizableData.hpp

    ${syntaxParser_SOURCE_DIR}/Core/WeightCalculator/WeightCalculator_Exponent.hpp

    ${syntaxParser_SOURCE_DIR}/Core/Interfaces/EmptyDataPreparer.h
    ${syntaxParser_SOURCE_DIR}/Core/Interfaces/EmptyRootFeatureCalculator.h

    ${syntaxParser_SOURCE_DIR}/En/EdgeCalculator/EdgeFeatureCalculatorEn.hpp
    ${syntaxParser_SOURCE_DIR}/En/EdgeCalculator/NodeFeatureCalculatorEn.cpp
    ${syntaxParser_SOURCE_DIR}/En/RootCalculator/RootFeatureCalculatorEn.hpp
    ${syntaxParser_SOURCE_DIR}/En/Settings/SyntaxSettingsEn.h

    ${syntaxParser_SOURCE_DIR}/Rus/DataPreparer/SyntaxDataPreparerRus.h
    ${syntaxParser_SOURCE_DIR}/Rus/EdgeCalculator/EdgeFeatureCalculatorRus.hpp
    ${syntaxParser_SOURCE_DIR}/Rus/EdgeCalculator/NodeFeatureCalculatorRus.h
    ${syntaxParser_SOURCE_DIR}/Rus/EdgeCalculator/SchemaCalculatorRus.h
    ${syntaxParser_SOURCE_DIR}/Rus/RootCalculator/RootFeatureCalculatorRus.hpp
    ${syntaxParser_SOURCE_DIR}/Rus/Settings/SyntaxSettingsRus.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/algorithms/CRF
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Chain
    ${PROJECT_ROOT_DIR}/algorithms/CRF/ChainTransformer
    ${PROJECT_ROOT_DIR}/algorithms/CRF/IO
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Model
    ${PROJECT_ROOT_DIR}/algorithms/CRF/PossibleStateFinder
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Regularization
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Settings
    ${PROJECT_ROOT_DIR}/algorithms/CRF/ViterbiNode

    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Clib
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/EventHandler
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer

    ${PROJECT_ROOT_DIR}/utils/FileManager

    ${PROJECT_ROOT_DIR}/nlp-stack/Dictionary
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Creator
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Disambiguator
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/FeatureCalculator
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/Core/Settings
    ${PROJECT_ROOT_DIR}/nlp-stack/SentenceSplitter/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Batches
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Creator
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Controller
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/DataPreparer
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Drawer
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Exceptions
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Learner
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Model
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Normalization
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/ParseAlgorithms
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Parser
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Scripts
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Templates
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Tester
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Tree
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/TreeOptimizable
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/WeightCalculator
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Core/Interfaces
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/En
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/En/EdgeCalculator
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/En/RootCalculator
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/En/Settings
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Rus
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Rus/Creator
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Rus/DataPreparer
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Rus/EdgeCalculator
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Rus/RootCalculator
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/Rus/Settings
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/Core

    ${PROJECT_ROOT_DIR}/utils/TagsetConverter
    ${PROJECT_ROOT_DIR}/utils/Tools
)

set(CMAKE_INCLUDE_SYSTEM_FLAG_${lang} "-isystem ")

include_directories(
    SYSTEM ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex
)

if (MSVC)
    find_package(OpenMP)
	if (OPENMP_FOUND)
		set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	endif()
else()
    set(OMP_LIB_NAME gomp)
endif()

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${syntaxParser_SOURCES} ${syntaxParser_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL syntaxParser_EXPORTS)
add_executable(${TARGET_NAME}-app ${syntaxParser_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools csvManager crf sentence-splitter tokenizer disambiguator optimization ${OMP_LIB_NAME})
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
