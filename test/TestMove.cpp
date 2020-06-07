#include <gtest/gtest.h>

#include "Move.hpp"

TEST(TestMove, UCIConstructor1) {
  auto move = chess::Move("a1a2");
  EXPECT_EQ(move.fromSquare, 7);
  EXPECT_EQ(move.toSquare, 15);
  EXPECT_FALSE(move.promotion);
}

TEST(TestMove, UCIConstructor2) {
  auto move = chess::Move("e2e4");
  EXPECT_EQ(move.fromSquare, 11);
  EXPECT_EQ(move.toSquare, 27);
  EXPECT_FALSE(move.promotion);
}

TEST(TestMove, UCIConstructor3) {
  auto move = chess::Move("a8h1");
  EXPECT_EQ(move.fromSquare, 63);
  EXPECT_EQ(move.toSquare, 0);
  EXPECT_FALSE(move.promotion);
}

TEST(TestMove, UCIConstructorPromotion) {
  auto move = chess::Move("a8h1q");
  EXPECT_EQ(move.fromSquare, 63);
  EXPECT_EQ(move.toSquare, 0);
  ASSERT_TRUE(move.promotion);
  EXPECT_EQ(move.promotion.value(), 'q');
}
