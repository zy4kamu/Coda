get_filename_component(project_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" project_name ${project_name})
string(TOLOWER ${project_name} project_name)

file(GLOB sources "*.c" "*/*.c" "*/*/*.c" "*.cpp" "*/*.cpp" "*/*/*.cpp")
file(GLOB headers "*.h" "*/*.h" "*/*/*.h")
file(GLOB sourceheaders "*.hpp" "*/*.hpp" "*/*/*.hpp")

ADD_DEFINITIONS("-Wall" "-fPIC")
