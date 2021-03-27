#include <gtest/gtest.h>

#include "Bitboard.hpp"

#define MOVE_IN(legalMoves, move) EXPECT_TRUE(std::find(legalMoves.begin(), legalMoves.end(), chess::Move(move)) != legalMoves.end())
#define MOVE_NOT_IN(legalMoves, move) EXPECT_TRUE(std::find(legalMoves.begin(), legalMoves.end(), chess::Move(move)) == legalMoves.end())

TEST(TestBitboard, parseStartposFen) {
  auto bitboard = chess::Bitboard();
    bitboard.parseFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  EXPECT_EQ(bitboard.getPawns(),   (0b11111111ull << 6u*8u) | (0b11111111ull << 1u*8u));
  EXPECT_EQ(bitboard.getBishops(), (0b00100100ull << 7u*8u) | (0b00100100ull << 0u*8u));
  EXPECT_EQ(bitboard.getKnights(), (0b01000010ull << 7u*8u) | (0b01000010ull << 0u*8u));
  EXPECT_EQ(bitboard.getRooks(),   (0b10000001ull << 7u*8u) | (0b10000001ull << 0u*8u));
  EXPECT_EQ(bitboard.getQueens(),  (0b00010000ull << 7u*8u) | (0b00010000ull << 0u*8u));
  EXPECT_EQ(bitboard.getKings(),   (0b00001000ull << 7u*8u) | (0b00001000ull << 0u*8u));

  EXPECT_TRUE(bitboard.getPov());
  EXPECT_TRUE(bitboard.getCastlingRights().all());
  EXPECT_EQ(bitboard.getEnPassantFile(), std::nullopt);
}

TEST(TestBitboard, parseStartpos) {
  auto bitboard = chess::Bitboard();
  bitboard.startpos();
  EXPECT_EQ(bitboard.getPawns(),   (0b11111111ull << 6u*8u) | (0b11111111ull << 1u*8u));
  EXPECT_EQ(bitboard.getBishops(), (0b00100100ull << 7u*8u) | (0b00100100ull << 0u*8u));
  EXPECT_EQ(bitboard.getKnights(), (0b01000010ull << 7u*8u) | (0b01000010ull << 0u*8u));
  EXPECT_EQ(bitboard.getRooks(),   (0b10000001ull << 7u*8u) | (0b10000001ull << 0u*8u));
  EXPECT_EQ(bitboard.getQueens(),  (0b00010000ull << 7u*8u) | (0b00010000ull << 0u*8u));
  EXPECT_EQ(bitboard.getKings(),   (0b00001000ull << 7u*8u) | (0b00001000ull << 0u*8u));
}

TEST(TestBitboard, parseStartposFenFlipped) {
  auto bitboard = chess::Bitboard();
    bitboard.parseFEN("RNBQKBNR/PPPPPPPP/8/8/8/8/pppppppp/rnbqkbnr w KQkq - 0 1");
  EXPECT_EQ(bitboard.getPawns(),   (0b11111111ull << 6u*8u) | (0b11111111ull << 1u*8u));
  EXPECT_EQ(bitboard.getBishops(), (0b00100100ull << 7u*8u) | (0b00100100ull << 0u*8u));
  EXPECT_EQ(bitboard.getKnights(), (0b01000010ull << 7u*8u) | (0b01000010ull << 0u*8u));
  EXPECT_EQ(bitboard.getRooks(),   (0b10000001ull << 7u*8u) | (0b10000001ull << 0u*8u));
  EXPECT_EQ(bitboard.getQueens(),  (0b00010000ull << 7u*8u) | (0b00010000ull << 0u*8u));
  EXPECT_EQ(bitboard.getKings(),   (0b00001000ull << 7u*8u) | (0b00001000ull << 0u*8u));
}

TEST(TestBitboard, parseSinglePiecePerRow) {
  auto bitboard = chess::Bitboard();
    bitboard.parseFEN("P7/1B6/2N5/3R4/4Q3/5K2/8/8 w KQkq - 0 1");
  EXPECT_EQ(bitboard.getPawns(), 1ull << (7u + 7u*8u));
  EXPECT_EQ(bitboard.getBishops(), 1ull << (6u + 6u*8u));
  EXPECT_EQ(bitboard.getKnights(), 1ull << (5u + 5u*8u));
  EXPECT_EQ(bitboard.getRooks(), 1ull << (4u + 4u*8u));
  EXPECT_EQ(bitboard.getQueens(), 1ull << (3u + 3u*8u));
  EXPECT_EQ(bitboard.getKings(), 1ull << (2u + 2u*8u));
}

TEST(TestBitboard, parseEmpty) {
  auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/8/8 w - - 0 1");
  EXPECT_EQ(bitboard.getPawns(), 0ull);
  EXPECT_EQ(bitboard.getBishops(), 0ull);
  EXPECT_EQ(bitboard.getKnights(), 0ull);
  EXPECT_EQ(bitboard.getRooks(), 0ull);
  EXPECT_EQ(bitboard.getQueens(), 0ull);
  EXPECT_EQ(bitboard.getKings(), 0ull);
}

TEST(TestBitboard, controllStartpos) {
  auto bitboard = chess::Bitboard();
  bitboard.parseFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");

  bitboard.evalAttack();
  EXPECT_EQ(bitboard.getControlled(), 0xffff7eull);
}

TEST(TestBitboard, controllKnight) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/8/7N b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getControlled(), 0x020400);
}
TEST(TestBitboard, controllQueen) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/8/7Q b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getControlled(), 0x81412111090503fe);
}
TEST(TestBitboard, controllRook) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/8/7R b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getControlled(), 0x01010101010101fe);
}
TEST(TestBitboard, controllBishop) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/8/7B b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getControlled(), 0x8040201008040200);
}
TEST(TestBitboard, controllKing) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/8/7K b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getControlled(), 0x302);
}

TEST(TestBitboard, controllXrayKing) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("k7/8/8/8/8/2K3r1/8/8 w - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getControlled(), 0x42c2020202fd0202ull);

}
TEST(TestBitboard, simplePin) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/2Q2p1k/8 b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getPinnedHorizontal(), 0x400);
}

TEST(TestBitboard, simpleNoPin) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/2Q2p2/7k b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getPinnedHorizontal(), 0x0);
}

TEST(TestBitboard, blockedPinInfront) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/2Q1Pp1k/8 b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getPinnedHorizontal(), 0x0);
}
TEST(TestBitboard, blockedPinBehind) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/2Q1pP1k/8 b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getPinnedHorizontal(), 0x0);
}


TEST(TestBitboard, blockedPinDouble) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/2Q2ppk/8 b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getPinnedHorizontal(), 0x0);
}

TEST(TestBitboard, check) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/2Q4k/8 b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getChecks(), 0x2000);
}

TEST(TestBitboard, doubleCheck) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/2Q4k/5N2 b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getChecks(), 0x2004);

}

TEST(TestBitboard, applyMoveStartpos) {
    auto bitboard = chess::Bitboard();
    bitboard.startpos();
    bitboard.applyMoveSelf(chess::Move("e2e4"));
    EXPECT_EQ(bitboard.getEnPassantFile(), std::nullopt);
    EXPECT_EQ(bitboard.getPawns(), 0x00ff00000800f700);
    EXPECT_EQ(bitboard.occupied(), 0xffff00000800f7ff);
    EXPECT_FALSE(bitboard.getPov());

    bitboard.applyMoveSelf(chess::Move("e7e5"));
    EXPECT_EQ(bitboard.getEnPassantFile(), std::nullopt);
    EXPECT_EQ(bitboard.getPawns(), 0x00f700080800f700);
    EXPECT_EQ(bitboard.occupied(), 0xfff700080800f7ff);
    EXPECT_TRUE(bitboard.getPov());
}

TEST(TestBitboard, applyMoveEnPassant) {
    auto bitboard = chess::Bitboard();
    bitboard.startpos();
    bitboard.applyMoveSelf(chess::Move("e2e4"));
    bitboard.applyMoveSelf(chess::Move("c7c5"));
    bitboard.applyMoveSelf(chess::Move("e4e5"));
    bitboard.applyMoveSelf(chess::Move("d7d5"));

    EXPECT_EQ(bitboard.getEnPassantFile(), 4);
    EXPECT_EQ(bitboard.getPawns(), 0x00cf00380000f700);
    EXPECT_EQ(bitboard.occupied(), 0xffcf00380000f7ff);
    EXPECT_TRUE(bitboard.getPov());

    bitboard.applyMoveSelf(chess::Move("e5d6"));

    EXPECT_EQ(bitboard.getEnPassantFile(), std::nullopt);
    EXPECT_EQ(bitboard.getPawns(), 0x00cf10200000f700);
    EXPECT_EQ(bitboard.occupied(), 0xffcf10200000f7ff);
    EXPECT_FALSE(bitboard.getPov());
}

TEST(TestBitboard, applyIllegalEnPassant) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("1k6/4p3/8/r4P1K/8/8/8/8 b - - 0 1");
    bitboard.applyMoveSelf(chess::Move("e7e5"));
    EXPECT_FALSE(bitboard.getEnPassantFile());
}

TEST(TestBitboard, kingMoves) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/5k2/8/4pn2/4P3/3K4/8/8 w - - 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(), 6);
    MOVE_IN(legalMoves, "d3e2");
    MOVE_IN(legalMoves, "d3d2");
    MOVE_IN(legalMoves, "d3c2");
    MOVE_IN(legalMoves, "d3c3");
    MOVE_IN(legalMoves, "d3c4");
    MOVE_IN(legalMoves, "e4f5");
}

TEST(TestBitboard, rookMoves) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("7k/8/8/8/8/7p/7P/6RK w - - 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(), 6+7);
    MOVE_IN(legalMoves, "g1g2");
    MOVE_IN(legalMoves, "g1g3");
    MOVE_IN(legalMoves, "g1g4");
    MOVE_IN(legalMoves, "g1g5");
    MOVE_IN(legalMoves, "g1g6");
    MOVE_IN(legalMoves, "g1g7");
    MOVE_IN(legalMoves, "g1g8");

    MOVE_IN(legalMoves, "g1a1");
    MOVE_IN(legalMoves, "g1b1");
    MOVE_IN(legalMoves, "g1c1");
    MOVE_IN(legalMoves, "g1d1");
    MOVE_IN(legalMoves, "g1e1");
    MOVE_IN(legalMoves, "g1f1");
}

TEST(TestBitboard, rookBlockCheck) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("7k/8/2b5/8/8/7p/7P/6RK w - - 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(), 1);
    MOVE_IN(legalMoves, "g1g2");
}
TEST(TestBitboard, knightPinned) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/6k1/2b5/3N4/8/7p/7P/7K w - - 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(), 1);
    MOVE_IN(legalMoves, "h1g1");
}
TEST(TestBitboard, bishopWeakPinned) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/6k1/2b5/3B4/8/7p/7P/7K w - - 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(), 5);
    MOVE_IN(legalMoves, "d5c6");
    MOVE_IN(legalMoves, "d5e4");
    MOVE_IN(legalMoves, "d5f3");
    MOVE_IN(legalMoves, "d5g2");

    MOVE_IN(legalMoves, "h1g1");
}

TEST(TestBitboard, knightBlockCheck) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("7k/8/2b5/8/5N2/7p/7P/6BK w - - 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(), 2);
    MOVE_IN(legalMoves, "f4g2");
    MOVE_IN(legalMoves, "f4d5");
}

TEST(TestBitboard, pawnPush) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/1pr4k/1P6/4n1P1/2pP4/7p/2P2P1P/7K w - - 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(), 8);
    MOVE_IN(legalMoves, "b6c7");
    MOVE_IN(legalMoves, "c2c3");
    MOVE_IN(legalMoves, "d4d5");
    MOVE_IN(legalMoves, "d4e5");
    MOVE_IN(legalMoves, "f2f3");
    MOVE_IN(legalMoves, "f2f4");
    MOVE_IN(legalMoves, "g5g6");
    MOVE_IN(legalMoves, "h1g1");
}

TEST(TestBitboard, pawnPromote) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("2b5/1P6/7k/8/8/8/5K2/8 w - - 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(), 8+8);
    MOVE_IN(legalMoves, "b7b8q");
    MOVE_IN(legalMoves, "b7b8r");
    MOVE_IN(legalMoves, "b7b8b");
    MOVE_IN(legalMoves, "b7b8n");

    MOVE_IN(legalMoves, "b7c8q");
    MOVE_IN(legalMoves, "b7c8r");
    MOVE_IN(legalMoves, "b7c8b");
    MOVE_IN(legalMoves, "b7c8n");

    MOVE_IN(legalMoves, "f2e1");
    MOVE_IN(legalMoves, "f2e2");
    MOVE_IN(legalMoves, "f2e3");

    MOVE_IN(legalMoves, "f2f1");
    MOVE_IN(legalMoves, "f2f3");


    MOVE_IN(legalMoves, "f2g1");
    MOVE_IN(legalMoves, "f2g2");
    MOVE_IN(legalMoves, "f2g3");
}

TEST(TestBitboard, legalCastling) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("4k3/8/8/8/8/p3p2p/P3P2P/R3K2R w KQ - 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(), 4 + 3 + 2);

    MOVE_IN(legalMoves, "a1b1");
    MOVE_IN(legalMoves, "a1c1");
    MOVE_IN(legalMoves, "a1d1");

    MOVE_IN(legalMoves, "e1c1");
    MOVE_IN(legalMoves, "e1d1");
    MOVE_IN(legalMoves, "e1f1");
    MOVE_IN(legalMoves, "e1g1");

    MOVE_IN(legalMoves, "h1f1");
    MOVE_IN(legalMoves, "h1g1");
}

TEST(TestBitboard, blockedCastling) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("3rkr2/8/8/8/8/p3p2p/P3P2P/R3K2R w KQ - 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(),  3 + 2);

    MOVE_IN(legalMoves, "a1b1");
    MOVE_IN(legalMoves, "a1c1");
    MOVE_IN(legalMoves, "a1d1");

    MOVE_IN(legalMoves, "h1f1");
    MOVE_IN(legalMoves, "h1g1");
}

TEST(TestBitboard, enPassantCaptureMoves) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/4k3/1p1pPp2/8/8/1K6 b - e3 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(),  9);

    MOVE_IN(legalMoves, "f4e3");
    MOVE_IN(legalMoves, "d4e3");
}

TEST(TestBitboard, enPassantRegularPin) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/4k3/1p1pPp2/8/7Q/1K6 b - e3 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(),  7);

    MOVE_NOT_IN(legalMoves, "f4e3");
    MOVE_IN(legalMoves, "d4e3");
}

TEST(TestBitboard, enPassantWeakPin) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/2k5/1p1pPp2/8/8/1K4Q1 b - e3 0 1");
    auto legalMoves = bitboard.legalMoves();
    EXPECT_EQ(legalMoves.size(),  9);

    MOVE_IN(legalMoves, "f4e3");
    MOVE_IN(legalMoves, "d4e3");

}


