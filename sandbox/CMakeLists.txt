add_executable(game application.cpp)

target_link_libraries(game PRIVATE engine)

target_compile_features(game PRIVATE cxx_std_20 )