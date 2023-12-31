cmake_minimum_required(VERSION 3.26)

project(Prsim LANGUAGES CXX C)

aux_source_directory(src/ SRC)
aux_source_directory(src/anim SRC)
aux_source_directory(src/engine SRC)
aux_source_directory(src/engine/imgui SRC)
aux_source_directory(src/geo SRC)
aux_source_directory(src/render SRC)
aux_source_directory(src/tools SRC)
aux_source_directory(src/config SRC)

add_library(engine STATIC ${SRC})
set(EXPORT_COMPILE_COMMANDS ON)

target_compile_features(engine PRIVATE cxx_std_17)
target_include_directories(engine PUBLIC include)

add_subdirectory(3rdlibs)

target_link_libraries(engine PUBLIC glfw imgui glm stb_image glad pecs assimp spdlog)


SET(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
SET(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Assimp build tests" FORCE)
SET(ASSIMP_INSTALL OFF CACHE BOOL "Assimp install" FORCE) 
SET(ASSIMP_INSTALL_PDB OFF CACHE BOOL "Assimp install PDB" FORCE)

SET(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW build document" FORCE) 
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW build tests" FORCE) 
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW build examples" FORCE)  

target_include_directories(engine PUBLIC 3rdlibs/glfw/include)
target_include_directories(engine PUBLIC 3rdlibs/imgui)
target_include_directories(engine PUBLIC 3rdlibs/glad/include)
target_include_directories(engine PUBLIC 3rdlibs/glm)
target_include_directories(engine PUBLIC 3rdlibs/pecs/include)
target_include_directories(engine PUBLIC 3rdlibs/assimp/include)
target_include_directories(engine PUBLIC 3rdlibs/spdlog/include)

add_subdirectory(sandbox)