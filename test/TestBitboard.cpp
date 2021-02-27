#include <gtest/gtest.h>

#include "Bitboard.hpp"


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

TEST(TestBitboard, simplePin) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/2Q2p1k/8 b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getPinned(), 0x400);
}

TEST(TestBitboard, simpleNoPin) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/2Q2p2/7k b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getPinned(), 0x0);
}

TEST(TestBitboard, blockedPinInfront) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/2Q1Pp1k/8 b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getPinned(), 0x0);
}
TEST(TestBitboard, blockedPinBehind) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/2Q1pP1k/8 b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getPinned(), 0x0);
}


TEST(TestBitboard, blockedPinDouble) {
    auto bitboard = chess::Bitboard();
    bitboard.parseFEN("8/8/8/8/8/8/2Q2ppk/8 b - - 0 1");
    bitboard.evalAttack();
    EXPECT_EQ(bitboard.getPinned(), 0x0);
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