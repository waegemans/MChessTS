#include <gtest/gtest.h>

#include "State.hpp"

TEST(TestState, QueenFree) {
  auto state = chess::State();
  state.parseFen("8/8/8/8/8/8/8/7Q");
  auto moves = state.legalMoves();
  EXPECT_EQ(moves.size(),21);

  state.parseFen("8/8/8/8/8/8/8/QQQQQQQQ");
  moves = state.legalMoves();
  EXPECT_EQ(moves.size(), 14*8);

  state.parseFen("Q7/1Q6/2Q5/3Q4/4Q3/5Q2/6Q1/7Q");
  moves = state.legalMoves();
  EXPECT_EQ(moves.size(), 14*8+24);

  state.parseFen("7Q/6Q1/5Q2/4Q3/3Q4/2Q5/1Q6/Q7");
  moves = state.legalMoves();
  EXPECT_EQ(moves.size(), 14*8+24);
}

TEST(TestState, QueenTake) {
  auto state = chess::State();
  state.parseFen("1p6/2p5/3p4/4p3/5p2/6p1/7p/QQQQQQQQ");
  auto moves = state.legalMoves();
  EXPECT_EQ(moves.size(),35 + 7*8 - 6);

  state.parseFen("5ppQ/4ppQp/3ppQpp/2ppQpp1/1ppQpp2/ppQpp3/pQpp4/Qpp5");
  moves = state.legalMoves();
  EXPECT_EQ(moves.size(), 6*6+2*2);
}

TEST(TestState, RookTake) {
  auto state = chess::State();
  state.parseFen("1p6/2p5/3p4/4p3/5p2/6p1/7p/RRRRRRRR");
  auto moves = state.legalMoves();
  EXPECT_EQ(moves.size(),35);

  state.parseFen("5ppR/4ppRp/3ppRpp/2ppRpp1/1ppRpp2/ppRpp3/pRpp4/Rpp5");
  moves = state.legalMoves();
  EXPECT_EQ(moves.size(), 7*4);
}

TEST(TestState, BishopTake) {
  auto state = chess::State();
  state.parseFen("1p6/2p5/3p4/4p3/5p2/6p1/7p/BBBBBBBB");
  auto moves = state.legalMoves();
  EXPECT_EQ(moves.size(),7*8-6);

  state.parseFen("5ppB/4ppBp/3ppBpp/2ppBpp1/1ppBpp2/ppBpp3/pBpp4/Bpp5");
  moves = state.legalMoves();
  EXPECT_EQ(moves.size(), 6*2);
}