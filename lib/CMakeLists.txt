set(
        CHESS_SOURCES

        "Bitboard.cpp"
        "State.cpp"
        "Move.cpp"
        eval/Evaluator.cpp eval/Evaluator.hpp eval/Score.cpp eval/Score.hpp eval/PieceCountEvaluator.cpp eval/PieceCountEvaluator.hpp)

add_library(
        chess_core
        STATIC
        ${CHESS_SOURCES}
)

target_include_directories(
        chess_core
        PUBLIC
        ${CMAKE_SOURCE_DIR}/lib
)