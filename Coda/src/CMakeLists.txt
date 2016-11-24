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

# src

# storage libraries
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(algorithms/CRF CRF)
add_subdirectory(algorithms/LogisticRegression LogisticRegression)
add_subdirectory(algorithms/Optimization Optimization)
add_subdirectory(algorithms/SVM SVM)

add_subdirectory(nlp-stack/Chunking Chunking)
add_subdirectory(nlp-stack/Dictionary Dictionary)
add_subdirectory(nlp-stack/Disambiguator Disambiguator)
add_subdirectory(nlp-stack/Lemmatizer Lemmatizer)
add_subdirectory(nlp-stack/PorterStemmer PorterStemmer)
add_subdirectory(nlp-stack/SentenceSplitter SentenceSplitter)
add_subdirectory(nlp-stack/Stresser Stresser)
add_subdirectory(nlp-stack/SyntaxParser SyntaxParser)
add_subdirectory(nlp-stack/Tokenizer Tokenizer)

add_subdirectory(utils/FileManager FileManager)
add_subdirectory(utils/Eigen Eigen)
add_subdirectory(utils/RegexpTools RegexpTools)
add_subdirectory(utils/StatCalc StatCalc)
add_subdirectory(utils/TagsetConstants TagsetConstants)
add_subdirectory(utils/TagsetConverter TagsetConverter)
add_subdirectory(utils/Tools Tools)









