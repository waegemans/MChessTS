#pragma once

#include "Bitboard.hpp"
#include "Move.hpp"

#include <bitset>
#include <unordered_set>
#include <vector>
#include <stack>

namespace chess {
class State {
private:
    Bitboard startingPosition;
    std::vector<Bitboard> stack;


public:
  void reset ();
  void parseFen(std::string_view);
  [[nodiscard]] const Bitboard& getCurrentBitboard() const;
  void pushMove(Move);
  void pushBoard(Bitboard&&);
  void popBoard();

  bool isTreefoldRepetition() const;
  bool isGameOver() const;
};
}
