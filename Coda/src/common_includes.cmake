get_filename_component(project_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" project_name ${project_name})
string(TOLOWER ${project_name} project_name)

file(GLOB sources "*.c" "*/*.c" "*/*/*.c" "*.cpp" "*/*.cpp" "*/*/*.cpp")
file(GLOB headers "*.h" "*/*.h" "*/*/*.h")
file(GLOB sourceheaders "*.hpp" "*/*.hpp" "*/*/*.hpp")

file(GLOB crf_subfolders
    ${PROJECT_ROOT_DIR}/algorithms/CRF/*/
    ${PROJECT_ROOT_DIR}/algorithms/CRF/*/*/)

file(GLOB disambiguator_subfolders
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/*/
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/*/*/)

file(GLOB tokenizer_subfolders
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/*/
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/*/*/)

file(GLOB sentence_splitter_subfolders
    ${PROJECT_ROOT_DIR}/nlp-stack/SentenceSplitter/*/
    ${PROJECT_ROOT_DIR}/nlp-stack/SentenceSplitter/*/*/)

file(GLOB syntax_parser_subfolders
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/*/
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/*/*/
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/*/*/*/)


ADD_DEFINITIONS("-Wall" "-fPIC")
