include (../common_includes.cmake)

include_directories(
    ${sentence_splitter_subfiles}
    ${tokenizer_subfolders}
    ${disambiguator_subfiles}
    ${syntax_parser_subfiles}
    ${crf_subfiles}
    ${dictionary_subfiles}
    ${PROJECT_ROOT_DIR}/algorithms/Optimization/Optimizer/
    ${PROJECT_ROOT_DIR}/utils/Tools/
    ${PROJECT_ROOT_DIR}/utils/TagsetConverter/
    ${PROJECT_ROOT_DIR}/utils/FileManager
)

add_library(${project_name} ${LIBRARY_TYPE} ${sources} ${headers})
target_link_libraries(${project_name} tools)
