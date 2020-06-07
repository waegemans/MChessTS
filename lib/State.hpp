#pragma once

#include "Bitboard.hpp"
#include "Move.hpp"

#include <bitset>
#include <unordered_set>

namespace chess {
class State {
private:
  Bitboard bitboard;
  bool pov;
  std::bitset<4> castlingRights;

  unsigned halfMoveCount;
  unsigned fullMoveCount;

public:
  void reset ();
  void parseFen(std::string_view);
  std::unordered_set<Move, HashMove> legalMoves();
};
}
