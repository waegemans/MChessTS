#include <gtest/gtest.h>

#include "search/AlphaBetaSearch.hpp"
#include "eval/PieceCountEvaluator.hpp"

namespace {
    void expectUCI(std::string_view fen, std::string uci) {
        chess::State state;
        chess::PieceCountEvaluator evaluator;
        chess::AlphaBetaSearch alphaBeta(evaluator);
        state.parseFen(fen);
        auto bestMove = alphaBeta.findNextMove(state,{0,0,0,0});
        EXPECT_EQ(bestMove.toUCI(), uci);
    }
}
TEST(TestAlphaBetaSearch, MateInOne) {
    expectUCI("6k1/8/6K1/8/3R4/8/8/8 w - - 0 1", "d4d8");
}
TEST(TestAlphaBetaSearch, DrawInOne) {
    expectUCI("3Qk3/8/8/5K2/8/8/8/8 b - - 0 1","e8d8");
    expectUCI("3Rk3/8/6K1/8/8/8/8/8 b - - 0 1", "e8d8");
}
TEST(TestAlphaBetaSearch, MateInTwo) {
    expectUCI("1k6/3q1Q2/1K6/8/8/8/8/8 w - - 0 1","f7d7");
}

TEST(TestAlphaBetaSearch, MateInTree) {
    expectUCI("8/kp4pp/6q1/1p6/2p3Q1/N1P2P2/PP1rr2P/3R1R1K b - - 13 30","e2h2");
}


