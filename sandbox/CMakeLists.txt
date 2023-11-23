aux_source_directory(src/ GAME_SRC)

add_executable(game ${GAME_SRC})

target_link_libraries(game PRIVATE engine)

target_compile_features(game PRIVATE cxx_std_20)