set(
        CHESS_SOURCES

        Bitboard.cpp
        Move.cpp
        State.cpp

        eval/Evaluator.cpp
        eval/PiecePositionEvaluator.cpp
        eval/Score.cpp

        search/AlphaBetaSearch.cpp
        search/Search.cpp

        wrapper/UCI.cpp
)

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