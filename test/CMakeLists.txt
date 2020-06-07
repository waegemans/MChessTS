set(
        TEST_SOURCES

        TestBitboard.cpp
        TestMove.cpp
        TestState.cpp
        Tester.cpp
)

add_executable(tester ${TEST_SOURCES})
target_link_libraries(
        tester
        PUBLIC
        chess_core
        GTest::GTest
)