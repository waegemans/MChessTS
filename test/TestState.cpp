#include <gtest/gtest.h>

#include "State.hpp"

namespace {
    void expect_fen_move_count(std::string_view fen, int move_count, bool onlyLegal=false) {
        auto state = chess::State();
        state.parseFen(fen);
        auto moves = onlyLegal ? state.legalMoves() : state.pseudoLegalMoves();
        EXPECT_EQ(moves.size(), move_count);
    }

    void expect_fen_contains_ucis(std::string_view fen, const std::vector<std::string>& uci_vector, bool onlyLegal=false) {
        auto state = chess::State();
        state.parseFen(fen);
        auto moves = onlyLegal ? state.legalMoves() : state.pseudoLegalMoves();
        for (const auto& uci : uci_vector) {
            auto move = chess::Move(uci);
            EXPECT_EQ(moves.count(move), 1);
        }
    }
}
TEST(TestState, ParsePov) {
    auto state = chess::State();
    state.parseFen("8/8/8/8/8/8/8/7Q w - - 0 1");
    EXPECT_TRUE(state.getPov());

    state.parseFen("8/8/8/8/8/8/8/7Q b - - 0 1");
    EXPECT_FALSE(state.getPov());
}
TEST(TestState, ParseCastling) {
    auto state = chess::State();
    state.parseFen("8/8/8/8/8/8/8/7Q w KQkq - 0 1");
    auto rights = state.getCastlingRights();
    EXPECT_TRUE(rights[0]);
    EXPECT_TRUE(rights[1]);
    EXPECT_TRUE(rights[2]);
    EXPECT_TRUE(rights[3]);
}
TEST(TestState, ParseEnPassant) {
    auto state = chess::State();
    state.parseFen("8/8/8/8/8/8/8/7Q w KQkq e3 0 1");
    auto enPassantFile = state.getEnPassantFile();
    ASSERT_TRUE(enPassantFile.has_value());
    EXPECT_EQ(enPassantFile.value(),3);

    state.parseFen("8/8/8/8/8/8/8/7Q w KQkq a3 0 1");
    enPassantFile = state.getEnPassantFile();
    ASSERT_TRUE(enPassantFile.has_value());
    EXPECT_EQ(enPassantFile.value(),7);

    state.parseFen("8/8/8/8/8/8/8/7Q w KQkq - 0 1");
    enPassantFile = state.getEnPassantFile();
    EXPECT_FALSE(enPassantFile.has_value());


}
TEST(TestState, QueenFree) {
    expect_fen_move_count("8/8/8/8/8/8/8/7Q w - - 0 1",21);
    expect_fen_move_count("8/8/8/8/8/8/8/QQQQQQQQ w - - 0 1",14*8);
    expect_fen_move_count("Q7/1Q6/2Q5/3Q4/4Q3/5Q2/6Q1/7Q w - - 0 1",14*8+24);
    expect_fen_move_count("7Q/6Q1/5Q2/4Q3/3Q4/2Q5/1Q6/Q7 w - - 0 1",14*8+24);
}

TEST(TestState, QueenTake) {
    expect_fen_move_count("1p6/2p5/3p4/4p3/5p2/6p1/7p/QQQQQQQQ w - - 0 1",35 + 7*8 - 6);
    expect_fen_move_count("5ppQ/4ppQp/3ppQpp/2ppQpp1/1ppQpp2/ppQpp3/pQpp4/Qpp5 w - - 0 1",6*6+2*2);
}

TEST(TestState, RookTake) {
    expect_fen_move_count("1p6/2p5/3p4/4p3/5p2/6p1/7p/RRRRRRRR w - - 0 1",35);
    expect_fen_move_count("5ppR/4ppRp/3ppRpp/2ppRpp1/1ppRpp2/ppRpp3/pRpp4/Rpp5 w - - 0 1",7*4);
}

TEST(TestState, BishopTake) {
    expect_fen_move_count("1p6/2p5/3p4/4p3/5p2/6p1/7p/BBBBBBBB w - - 0 1",7*8-6);
    expect_fen_move_count("5ppB/4ppBp/3ppBpp/2ppBpp1/1ppBpp2/ppBpp3/pBpp4/Bpp5 w - - 0 1",6*2);
}

TEST(TestState, PawnStart) {
    expect_fen_move_count("8/8/8/8/8/8/P7/8 w - - 0 1",2);
    expect_fen_move_count("8/8/8/8/8/P7/8/8 w - - 0 1",1);
    expect_fen_move_count("8/8/8/8/k7/8/P7/8 w - - 0 1",1);
}

TEST(TestState, PawnEnemy) {
    expect_fen_move_count("1k6/8/8/2p5/8/5K2/8/8 w - - 0 1",8);
}

TEST(TestState, PawnTake) {
    expect_fen_move_count("8/8/8/8/1k6/P7/8/8 w - - 0 1",2);
    expect_fen_move_count("8/8/8/8/8/1k6/P7/8 w - - 0 1",3);
    expect_fen_move_count("8/8/8/8/8/pppppppp/PPPPPPPP/8 w - - 0 1",7*2);
}

TEST(TestState, PawnPromote) {
    expect_fen_move_count("8/P7/8/8/8/8/8/8 w - - 0 1",4);
    expect_fen_move_count("pppppppp/PPPPPPPP/8/8/8/8/8/8 w - - 0 1",7*2*4);
}

TEST(TestState, KingFree) {
    expect_fen_move_count("8/8/4K3/8/8/8/8/8 w - - 0 1",8);
}

TEST(TestState, KingRestricted) {
    expect_fen_move_count("K7/8/8/8/8/8/8/8 w - - 0 1",3);
    expect_fen_move_count("k7/8/8/8/8/8/8/8 b - - 0 1",3);
    expect_fen_move_count("Kp6/pp6/8/8/8/8/8/8 w - - 0 1",3);
    expect_fen_move_count("KR6/PP6/8/8/8/8/8/8 w - - 0 1",6);
}

TEST(TestState, KnightFree) {
    expect_fen_move_count("8/8/8/3N4/8/8/8/8 w - - 0 1",8);
    expect_fen_move_count("8/8/3ppp2/3pNP2/3PPP2/8/8/8 w - - 0 1",10);
}
TEST(TestState, KnightCapture) {
    expect_fen_move_count("8/4r3/1r3r2/3N4/1q3b2/4p3/8/8 w - - 0 1",8);
}
TEST(TestState, KnightRestricted) {
expect_fen_move_count("8/3R4/2R5/4N3/6R1/5R2/8/8 w - - 0 1",7*2*4+4);
expect_fen_move_count("8/3P1P2/2P3P1/4N3/2P3P1/3P1P2/8/8 w - - 0 1",6+4*2);
}

TEST(TestState, EnPassantSimple) {
    expect_fen_move_count("4k3/8/8/8/1pP5/8/8/6K1 b - c3 0 1",7);
    expect_fen_contains_ucis("4k3/8/8/8/1pP5/8/8/6K1 b - c3 0 1",{"b4b3","b4c3"});

    expect_fen_move_count("4k3/8/8/8/2Pp4/8/8/6K1 b - c3 0 1",7);
    expect_fen_contains_ucis("4k3/8/8/8/2Pp4/8/8/6K1 b - c3 0 1",{"d4d3","d4c3"});
}

TEST(TestState, CastleSimple) {
    expect_fen_move_count("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1",19+7);

    expect_fen_move_count("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1",22);
    expect_fen_contains_ucis("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1",{"e1g1"});

    expect_fen_move_count("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1",22);
    expect_fen_contains_ucis("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1",{"e8g8"});

    expect_fen_move_count("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3KBNR w KQkq - 0 1",23);
    expect_fen_contains_ucis("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3KBNR w KQkq - 0 1",{"e1c1"});

    expect_fen_move_count("r3kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1",23);
    expect_fen_contains_ucis("r3kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1",{"e8c8"});
}

TEST(TestState, OwnMate) {
    expect_fen_move_count("1k6/4r1r1/3r4/5K2/8/8/8/8 w - - 0 1", 1, true);
}

TEST(TestState, EnPassant) {
    expect_fen_move_count("k7/8/8/8/1pP5/8/8/7K b - c3 0 1", 5);
    expect_fen_contains_ucis("k7/8/8/8/1pP5/8/8/7K b - c3 0 1", {"b4c3"});

    expect_fen_move_count("k7/8/8/8/2Pp4/8/8/7K b - c3 0 1", 5);
}

TEST(TestState, EnPassantMateBlocked) {
    expect_fen_move_count("8/8/8/8/kpP4R/8/6K1/8 b - c3 0 1", 4, true);
}

TEST(TestState, RestrictedCaslte) {
    expect_fen_move_count("1k1r1r2/8/8/8/8/8/8/R3K2R w KQ - 0 1", 20, true);
}

TEST(TestState, ActualError1) {
    chess::State state;
    state.reset();
    std::vector<std::string> moveList {"e2e4", "d7d6", "g1f3", "c8d7", "d2d4", "d7a4", "b2b3", "a4b3", "c2b3", "d8c8", "b1c3", "b8d7", "f1c4", "h7h5", "c1f4", "a8b8", "e1g1", "g8h6", "e4e5", "d7e5", "d4e5", "g7g5", "f4g5", "h6f5", "e5d6", "c7d6", "f1e1", "a7a6", "g5e7", "c8c4", "b3c4"};
    for (auto const& uciStr : moveList) {
        chess::Move move (uciStr);
        state.applyMoveSelf(move);
    }

    chess::State state2;
    state2.parseFen("1r2kb1r/1p2Bp2/p2p4/5n1p/2P5/2N2N2/P4PPP/R2QR1K1 b k - 0 16");

    EXPECT_EQ(state,state2);
}
TEST(TestState, ActualError2) {
    expect_fen_move_count("2b3q1/2pkpp2/p2p4/3P1P1p/7P/n5P1/1p4K1/8 b - - 2 44",6+4+2+1+4+10,true);//pawn+promote+king+bishop+knight+queen
}

TEST(TestState, ApplyCastling) {
    chess::State state,endState;
    state.parseFen("1rq1kbnr/pppnppp1/3p4/7p/2BPPB2/1PN2N2/P4PPP/R2QK2R w KQk - 2 9");
    endState.parseFen("1rq1kbnr/pppnppp1/3p4/7p/2BPPB2/1PN2N2/P4PPP/R2Q1RK1 b k - 3 9");

    state.applyMoveSelf(chess::Move("e1g1"));
    EXPECT_EQ(state,endState);

}

TEST(TestState, PromotionQueen) {
    auto state = chess::State();
    state.parseFen("8/5P2/1k6/8/6K1/8/8/8 w - - 0 1");
    state.applyMoveSelf(chess::Move("f7f8q"));
    state.applyMoveSelf(chess::Move("b6b5"));
    auto moves = state.legalMoves();
    EXPECT_EQ(moves.size(), 29);
}
TEST(TestState, PromotionRook) {
    auto state = chess::State();
    state.parseFen("8/5P2/1k6/8/6K1/8/8/8 w - - 0 1");
    state.applyMoveSelf(chess::Move("f7f8r"));
    state.applyMoveSelf(chess::Move("b6b5"));
    auto moves = state.legalMoves();
    EXPECT_EQ(moves.size(), 22);
}
TEST(TestState, PromotionBishop) {
    auto state = chess::State();
    state.parseFen("8/5P2/1k6/8/6K1/8/7p/8 w - - 0 1");
    state.applyMoveSelf(chess::Move("f7f8b"));
    state.applyMoveSelf(chess::Move("b6b5"));
    auto moves = state.legalMoves();
    EXPECT_EQ(moves.size(), 7+8);
}
TEST(TestState, PromotionKnight) {
    auto state = chess::State();
    state.parseFen("8/5P2/1k6/8/6K1/8/7p/8 w - - 0 1");
    state.applyMoveSelf(chess::Move("f7f8n"));
    state.applyMoveSelf(chess::Move("b6b5"));
    auto moves = state.legalMoves();
    EXPECT_EQ(moves.size(), 12);
}

TEST(TestState, GameOver) {
    chess::State state;
    state.parseFen("3Qk3/8/8/5K2/8/8/8/8 b - - 0 1");
    EXPECT_FALSE(state.isGameOver());
    state.parseFen("3Rk3/8/6K1/8/8/8/8/8 b - - 0 1");
    EXPECT_FALSE(state.isGameOver());
}

TEST(TestState, Checked) {
    chess::State state;
    state.parseFen("8/kp4pp/8/1p6/2p3q1/N1P2P2/PP1rr2P/3R1R1K w - - 0 1");
    EXPECT_FALSE(state.isCheck());
    state.parseFen("8/kp4pp/6q1/1p6/2p3Q1/N1P2P2/PP1r3r/3R1R1K w - - 0 1");
    EXPECT_TRUE(state.isCheck());
}
