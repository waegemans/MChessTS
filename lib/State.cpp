#include "State.hpp"

#include <cassert>

namespace chess {
namespace {

}

void State::reset() {
  bitboard.startpos();
  pov = true;
  castlingRights.reset();
}

void State::parseFen(std::string_view fen) {
  bitboard.parse_fen(fen);
  pov = true;
  castlingRights.reset();
}

std::unordered_set<Move, HashMove> State::legalMoves() {
  std::unordered_set<Move, HashMove> result;
  auto orthogonalPieces = bitboard.getRooks() | bitboard.getQueens();
  auto diagonalPieces = bitboard.getBishops() | bitboard.getQueens();

  for (int xscale = -1; xscale < 2; xscale++) {
    for (int yscale = -1; yscale < 2; yscale++) {
      if (yscale==0 && xscale==0)
        continue;
      bool povDirection = yscale == (pov ? 1 : -1);
      bool orthogonal = yscale == 0 || xscale == 0;
      auto majorPieces = orthogonal ? orthogonalPieces : diagonalPieces;
      uint64_t lastFree = majorPieces;
      for (int i = 1; i < 8; i++) {
        auto dx = xscale*i;
        auto dy = yscale*i;
        auto free = bitboard.freeSquares(dy, dx, pov);
        auto take = bitboard.takeSquares(dy, dx, pov);
        uint64_t movable = lastFree & (free | take);
        if (i == 1) {
          movable |= bitboard.getKings() & (free | take);
        }
        lastFree &= lastFree & free;

        for (unsigned bitPos = 0; bitPos < 64; bitPos++) {
          if (((1ull << bitPos) & movable )!= 0) {
            auto toPos = bitPos-dx+dy*8;
            assert(toPos >= 0);
            assert(toPos < 64);
            result.emplace(bitPos,toPos);
          }
        }
      }
    }
  }
  return result;
}
}
