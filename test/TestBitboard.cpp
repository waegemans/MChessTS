#include <gtest/gtest.h>

#include "Bitboard.hpp"


TEST(TestBitboard, parseStartposFen) {
  auto bitboard = chess::Bitboard();
  bitboard.parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
  EXPECT_EQ(bitboard.getPawns(),   (0b11111111ull << 6u*8u) | (0b11111111ull << 1u*8u));
  EXPECT_EQ(bitboard.getBishops(), (0b00100100ull << 7u*8u) | (0b00100100ull << 0u*8u));
  EXPECT_EQ(bitboard.getKnights(), (0b01000010ull << 7u*8u) | (0b01000010ull << 0u*8u));
  EXPECT_EQ(bitboard.getRooks(),   (0b10000001ull << 7u*8u) | (0b10000001ull << 0u*8u));
  EXPECT_EQ(bitboard.getQueens(),  (0b00010000ull << 7u*8u) | (0b00010000ull << 0u*8u));
  EXPECT_EQ(bitboard.getKings(),   (0b00001000ull << 7u*8u) | (0b00001000ull << 0u*8u));
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
  bitboard.parse_fen("RNBQKBNR/PPPPPPPP/8/8/8/8/pppppppp/rnbqkbnr");
  EXPECT_EQ(bitboard.getPawns(),   (0b11111111ull << 6u*8u) | (0b11111111ull << 1u*8u));
  EXPECT_EQ(bitboard.getBishops(), (0b00100100ull << 7u*8u) | (0b00100100ull << 0u*8u));
  EXPECT_EQ(bitboard.getKnights(), (0b01000010ull << 7u*8u) | (0b01000010ull << 0u*8u));
  EXPECT_EQ(bitboard.getRooks(),   (0b10000001ull << 7u*8u) | (0b10000001ull << 0u*8u));
  EXPECT_EQ(bitboard.getQueens(),  (0b00010000ull << 7u*8u) | (0b00010000ull << 0u*8u));
  EXPECT_EQ(bitboard.getKings(),   (0b00001000ull << 7u*8u) | (0b00001000ull << 0u*8u));
}

TEST(TestBitboard, parseSinglePiecePerRow) {
  auto bitboard = chess::Bitboard();
  bitboard.parse_fen("P7/1B6/2N5/3R4/4Q3/5K2/8/8");
  EXPECT_EQ(bitboard.getPawns(), 1ull << (7u + 7u*8u));
  EXPECT_EQ(bitboard.getBishops(), 1ull << (6u + 6u*8u));
  EXPECT_EQ(bitboard.getKnights(), 1ull << (5u + 5u*8u));
  EXPECT_EQ(bitboard.getRooks(), 1ull << (4u + 4u*8u));
  EXPECT_EQ(bitboard.getQueens(), 1ull << (3u + 3u*8u));
  EXPECT_EQ(bitboard.getKings(), 1ull << (2u + 2u*8u));
}

TEST(TestBitboard, parseEmpty) {
  auto bitboard = chess::Bitboard();
  bitboard.parse_fen("8/8/8/8/8/8/8/8");
  EXPECT_EQ(bitboard.getPawns(), 0ull);
  EXPECT_EQ(bitboard.getBishops(), 0ull);
  EXPECT_EQ(bitboard.getKnights(), 0ull);
  EXPECT_EQ(bitboard.getRooks(), 0ull);
  EXPECT_EQ(bitboard.getQueens(), 0ull);
  EXPECT_EQ(bitboard.getKings(), 0ull);
}

TEST(TestBitboard, checkSquaresFile) {
  auto bitboard = chess::Bitboard();
  bitboard.parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
  for (int i = 0; i < 8; i++) {
    EXPECT_EQ(bitboard.freeSquares(-i,0,true), 0);
    EXPECT_EQ(bitboard.freeSquares(i,0,false), 0);

    EXPECT_EQ(bitboard.takeSquares(-i,0,true), 0);
    EXPECT_EQ(bitboard.takeSquares(i,0,false), 0);
  }

  EXPECT_EQ(bitboard.freeSquares(1,0,true), 0xff00ull);
  EXPECT_EQ(bitboard.freeSquares(2,0,true), 0xffffull);
  EXPECT_EQ(bitboard.freeSquares(3,0,true), 0xffffull);
  EXPECT_EQ(bitboard.freeSquares(4,0,true), 0xffffull);
  EXPECT_EQ(bitboard.freeSquares(5,0,true), 0x00ffull);
  EXPECT_EQ(bitboard.freeSquares(6,0,true), 0);
  EXPECT_EQ(bitboard.freeSquares(7,0,true), 0);

  EXPECT_EQ(bitboard.takeSquares(1,0,true), 0);
  EXPECT_EQ(bitboard.takeSquares(2,0,true), 0);
  EXPECT_EQ(bitboard.takeSquares(3,0,true), 0);
  EXPECT_EQ(bitboard.takeSquares(4,0,true), 0);
  EXPECT_EQ(bitboard.takeSquares(5,0,true), 0xff00ull);
  EXPECT_EQ(bitboard.takeSquares(6,0,true), 0xffffull);
  EXPECT_EQ(bitboard.takeSquares(7,0,true), 0x00ffull);

  EXPECT_EQ(bitboard.freeSquares(-1,0,false),(0x00ffull << 6u*8u));
  EXPECT_EQ(bitboard.freeSquares(-2,0,false),(0xffffull << 6u*8u));
  EXPECT_EQ(bitboard.freeSquares(-3,0,false),(0xffffull << 6u*8u));
  EXPECT_EQ(bitboard.freeSquares(-4,0,false),(0xffffull << 6u*8u));
  EXPECT_EQ(bitboard.freeSquares(-5,0,false),(0xff00ull << 6u*8u));
  EXPECT_EQ(bitboard.freeSquares(-6,0,false),0);
  EXPECT_EQ(bitboard.freeSquares(-7,0,false),0);

  EXPECT_EQ(bitboard.takeSquares(-1,0,false),0);
  EXPECT_EQ(bitboard.takeSquares(-2,0,false),0);
  EXPECT_EQ(bitboard.takeSquares(-3,0,false),0);
  EXPECT_EQ(bitboard.takeSquares(-4,0,false),0);
  EXPECT_EQ(bitboard.takeSquares(-5,0,false),(0x00ffull << 6u*8u));
  EXPECT_EQ(bitboard.takeSquares(-6,0,false),(0xffffull << 6u*8u));
  EXPECT_EQ(bitboard.takeSquares(-7,0,false),(0xff00ull << 6u*8u));
}
