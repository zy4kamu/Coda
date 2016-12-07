get_filename_component(project_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" project_name ${project_name})
string(TOLOWER ${project_name} project_name)

file(GLOB sources "*.c" "*/*.c" "*/*/*.c" "*.cpp" "*/*.cpp" "*/*/*.cpp")
file(GLOB headers "*.h" "*/*.h" "*/*/*.h")
file(GLOB sourceheaders "*.hpp" "*/*.hpp" "*/*/*.hpp")

macro(only_folders files result)
    set(cleaned_list "")
    foreach(file ${files})
        if(IS_DIRECTORY ${file})
            list(APPEND cleaned_list ${file})
        endif()
    endforeach()
    SET(${result} ${cleaned_list})
endmacro()

file(GLOB tools_subfiles
    ${PROJECT_ROOT_DIR}/utils/Tools/*/
    ${PROJECT_ROOT_DIR}/utils/Tools/*/*/
    ${PROJECT_ROOT_DIR}/utils/Tools/*/*/*/)
only_folders("${tools_subfiles}" tools_subfolders)

file(GLOB crf_subfiles
    ${PROJECT_ROOT_DIR}/algorithms/CRF/*/
    ${PROJECT_ROOT_DIR}/algorithms/CRF/*/*/)
only_folders("${crf_subfiles}" crf_subfolders)

file(GLOB disambiguator_subfiles
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/*/
    ${PROJECT_ROOT_DIR}/nlp-stack/Disambiguator/*/*/)
only_folders("${disambiguator_subfiles}" disambiguator_subfolders)

file(GLOB tokenizer_subfiles
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/*/
    ${PROJECT_ROOT_DIR}/nlp-stack/Tokenizer/*/*/)
only_folders("${tokenizer_subfiles}" tokenizer_subfolders)

file(GLOB sentence_splitter_subfiles
    ${PROJECT_ROOT_DIR}/nlp-stack/SentenceSplitter/*/
    ${PROJECT_ROOT_DIR}/nlp-stack/SentenceSplitter/*/*/)
only_folders("${sentence_splitter_subfiles}" sentence_splitter_subfolders)

file(GLOB syntax_parser_subfiles
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/*/
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/*/*/
    ${PROJECT_ROOT_DIR}/nlp-stack/SyntaxParser/*/*/*/)
only_folders("${syntax_parser_subfiles}" syntax_parser_subfolders)


ADD_DEFINITIONS("-Wall" "-fPIC")
