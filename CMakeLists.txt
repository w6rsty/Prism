cmake_minimum_required(VERSION 3.26)

project(Prsim LANGUAGES CXX C)

aux_source_directory(src/ SRC)
aux_source_directory(src/render SRC)

add_executable(engine ${SRC})
set(EXPORT_COMPILE_COMMANDS ON)

target_compile_features(engine PRIVATE cxx_std_17)
target_include_directories(engine PUBLIC include)
target_include_directories(engine PUBLIC include/render)
target_include_directories(engine PRIVATE include/scene)
target_include_directories(engine PRIVATE include/anim)

add_subdirectory(3rdlibs)

target_link_libraries(engine PUBLIC glfw imgui glm stb_image glad pecs assimp)

target_include_directories(engine PUBLIC 3rdlibs/glfw/include)
target_include_directories(engine PUBLIC 3rdlibs/imgui)
target_include_directories(engine PUBLIC 3rdlibs/glad/include)
target_include_directories(engine PUBLIC 3rdlibs/glm)
target_include_directories(engine PUBLIC 3rdlibs/pecs/include)
target_include_directories(engine PUBLIC 3rdlibs/assimp/include)