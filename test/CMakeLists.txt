set(
        TEST_SOURCES

        TestAlphaBetaSearch.cpp
        TestBitboard.cpp
        TestMove.cpp
        TestScore.cpp
        Tester.cpp)

add_executable(tester ${TEST_SOURCES})
target_link_libraries(
        tester
        PUBLIC
        chess_core
        GTest::GTest
)