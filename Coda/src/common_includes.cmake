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

ADD_DEFINITIONS("-Wall" "-fPIC")
