set(
        CHESS_SOURCES

        "Bitboard.cpp"
        "Move.cpp")

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