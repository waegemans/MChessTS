add_executable(
        chess_main
        main.cpp
)

target_link_libraries(
        chess_main
        PUBLIC
        chess_core
)