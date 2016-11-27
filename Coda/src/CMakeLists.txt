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

add_subdirectory(algorithms/CRF)
add_subdirectory(algorithms/LogisticRegression)
add_subdirectory(algorithms/Optimization)
add_subdirectory(algorithms/SVM)

add_subdirectory(nlp-stack/Chunking)
add_subdirectory(nlp-stack/Dictionary)
add_subdirectory(nlp-stack/Disambiguator)
add_subdirectory(nlp-stack/Lemmatizer)
add_subdirectory(nlp-stack/PorterStemmer)
add_subdirectory(nlp-stack/SentenceSplitter)
add_subdirectory(nlp-stack/Stresser)
add_subdirectory(nlp-stack/SyntaxParser)
add_subdirectory(nlp-stack/Tokenizer)

add_subdirectory(utils/FileManager)
add_subdirectory(utils/Eigen)
add_subdirectory(utils/RegexpTools)
add_subdirectory(utils/StatCalc)
add_subdirectory(utils/TagsetConstants)
add_subdirectory(utils/TagsetConverter)
add_subdirectory(utils/Tools)









