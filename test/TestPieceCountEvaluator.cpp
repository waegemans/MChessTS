#include <gtest/gtest.h>

#include "State.hpp"
#include "eval/PieceCountEvaluator.hpp"
namespace {
    void expectScore(const chess::Score expectedScore, std::string_view fen) {
        auto evaluator = chess::PieceCountEvaluator();
        auto state = chess::State();
        state.parseFen(fen);
        auto score = evaluator(state);
        EXPECT_EQ(score,expectedScore);
    }
}
TEST(TestPieceCountEvaluator, Mate) {
    expectScore({true, -1}, "rnb1kbnr/pppp1ppp/4p3/8/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3");
    expectScore({true, 1}, "rnbqkbnr/2pppQpp/1p6/p7/2B5/4P3/PPPP1PPP/RNB1K1NR b KQkq - 0 4");
}

TEST(TestPieceCountEvaluator, Startpos) {
    auto evaluator = chess::PieceCountEvaluator();
    auto state = chess::State();
    auto score = evaluator(state);
    EXPECT_EQ(chess::Score(0), score);
}

TEST(TestPieceCountEvaluator, Endgame) {
    expectScore({false, 620 - 500}, "8/1k1r4/8/8/8/2B1NK2/8/8 b - - 1 1");
    expectScore({false, 600}, "8/1k1r4/2b5/1p4Q1/8/2B1NK2/8/8 w - - 0 1");
}

