add_executable(
        chess_uci
        main.cpp)

target_link_libraries(
        chess_uci
        PUBLIC
        chess_core
)