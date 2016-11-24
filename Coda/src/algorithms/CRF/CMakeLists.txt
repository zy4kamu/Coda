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

set(TARGET_NAME crf)
set(crf_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(crf_SOURCES
    ${crf_SOURCE_DIR}/Chain/Chain.cpp
    ${crf_SOURCE_DIR}/Chain/ChainOptimizable.cpp

    ${crf_SOURCE_DIR}/ChainTransformer/ConjunctionsChainTransformer.cpp
    ${crf_SOURCE_DIR}/ChainTransformer/DoublingChainTransformer.cpp

    ${crf_SOURCE_DIR}/Controller/Controller.cpp

    ${crf_SOURCE_DIR}/Inference/CRFApplier.cpp

    ${crf_SOURCE_DIR}/IO/CRFBinaryReader.cpp
    ${crf_SOURCE_DIR}/IO/CRFBinaryWriter.cpp
    ${crf_SOURCE_DIR}/IO/CRFTextReader.cpp
    
    ${crf_SOURCE_DIR}/Model/CRF.cpp
    ${crf_SOURCE_DIR}/Model/Factors.cpp
    ${crf_SOURCE_DIR}/Model/State.cpp
    ${crf_SOURCE_DIR}/Model/VectorFactorsExtractor.cpp
    
    ${crf_SOURCE_DIR}/PossibleStateFinder/DefaultPossibleStateFinder.cpp
    ${crf_SOURCE_DIR}/PossibleStateFinder/DoublingPossibleStateFinder.cpp
    ${crf_SOURCE_DIR}/PossibleStateFinder/EmptyPossibleStateFinder.cpp
    
    ${crf_SOURCE_DIR}/Regularization/L2Regularization.cpp
    ${crf_SOURCE_DIR}/Regularization/RelearnRegularization.cpp

    ${crf_SOURCE_DIR}/Settings/CRFSettingsHolder.cpp
    
    ${crf_SOURCE_DIR}/Tester/ChainCollectionTester.cpp
    ${crf_SOURCE_DIR}/Tester/UnitLabelChainCollectionTester.cpp
	
    ${crf_SOURCE_DIR}/ViterbiNode/AbstractViterbiNode.cpp
    ${crf_SOURCE_DIR}/ViterbiNode/ViterbiNBestNode.cpp
    ${crf_SOURCE_DIR}/ViterbiNode/ViterbiNode.cpp
    ${crf_SOURCE_DIR}/ViterbiNode/ViterbiPercentageNode.cpp
    ${crf_SOURCE_DIR}/ViterbiNode/ZViterbiNode.cpp
    
    ${crf_SOURCE_DIR}/main.cpp
)

set(crf_HEADERS
    ${crf_SOURCE_DIR}/Chain/Chain.h
    ${crf_SOURCE_DIR}/Chain/ChainOptimizable.h

    ${crf_SOURCE_DIR}/ChainTransformer/ConjunctionsChainTransformer.h
    ${crf_SOURCE_DIR}/ChainTransformer/DefaultChainTransformer.h
    ${crf_SOURCE_DIR}/ChainTransformer/DoublingChainTransformer.h
    ${crf_SOURCE_DIR}/ChainTransformer/IChainTransformer.h
    ${crf_SOURCE_DIR}/ChainTransformer/PipeChainTransformer.h

    ${crf_SOURCE_DIR}/Controller/Controller.h

    ${crf_SOURCE_DIR}/Inference/CRFApplier.h

    ${crf_SOURCE_DIR}/IO/CRFBinaryReader.h
    ${crf_SOURCE_DIR}/IO/CRFBinaryWriter.h
    ${crf_SOURCE_DIR}/IO/CRFTextReader.h
    
    ${crf_SOURCE_DIR}/Learner/ChainCollectionLearner.h
    ${crf_SOURCE_DIR}/Learner/ChainCollectionReLearner.h
    
    ${crf_SOURCE_DIR}/Model/CRF.h
    ${crf_SOURCE_DIR}/Model/Factors.h
    ${crf_SOURCE_DIR}/Model/State.h
    ${crf_SOURCE_DIR}/Model/VectorFactorsExtractor.h
    
    ${crf_SOURCE_DIR}/PossibleStateFinder/DefaultPossibleStateFinder.h
    ${crf_SOURCE_DIR}/PossibleStateFinder/DoublingPossibleStateFinder.h
    ${crf_SOURCE_DIR}/PossibleStateFinder/EmptyPossibleStateFinder.h
    ${crf_SOURCE_DIR}/PossibleStateFinder/IPossibleStateFinder.h
    
    ${crf_SOURCE_DIR}/Regularization/IRegularization.h
    ${crf_SOURCE_DIR}/Regularization/L2Regularization.h
    ${crf_SOURCE_DIR}/Regularization/RelearnRegularization.h

    ${crf_SOURCE_DIR}/Settings/ICRFSettings.h
    ${crf_SOURCE_DIR}/Settings/CRFSettingsHolder.h
    
    ${crf_SOURCE_DIR}/Tester/ChainCollectionTester.h
    ${crf_SOURCE_DIR}/Tester/UnitLabelChainCollectionTester.h
    
    ${crf_SOURCE_DIR}/ViterbiNode/AbstractViterbiNode.h 
    ${crf_SOURCE_DIR}/ViterbiNode/ViterbiNode.h
    ${crf_SOURCE_DIR}/ViterbiNode/ViterbiNBestNode.h
    ${crf_SOURCE_DIR}/ViterbiNode/ViterbiPathItem.h
    ${crf_SOURCE_DIR}/ViterbiNode/ViterbiPercentageNode.h
    ${crf_SOURCE_DIR}/ViterbiNode/ZViterbiNode.h
)

include_directories(
    ${PROJECT_ROOT_DIR}/algorithms/CRF
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Chain
    ${PROJECT_ROOT_DIR}/algorithms/CRF/ChainTransformer
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Controller
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Inference
    ${PROJECT_ROOT_DIR}/algorithms/CRF/IO
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Learner
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Model
    ${PROJECT_ROOT_DIR}/algorithms/CRF/PossibleStateFinder
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Regularization
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Settings
    ${PROJECT_ROOT_DIR}/algorithms/CRF/Tester
    ${PROJECT_ROOT_DIR}/algorithms/CRF/ViterbiNode
    ${PROJECT_ROOT_DIR}/utils/FileManager
    ${PROJECT_ROOT_DIR}/utils/Tools
    ${PROJECT_ROOT_DIR}/utils/Tools/Strings

    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Clib
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/EventHandler
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer
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

add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${crf_SOURCES} ${crf_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL crf_EXPORTS)
add_executable(${TARGET_NAME}-app ${crf_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME} tools optimization csvManager ${OMP_LIB_NAME})
target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
ADD_DEFINITIONS("-Wall" "-fPIC")
