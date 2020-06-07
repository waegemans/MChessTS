#include "State.hpp"

#include <cassert>

namespace chess {
namespace {
uint64_t getMovablesQueenLike(int dx, int dy, int i, bool pov, bool isPovDirection, uint64_t& lastFree, const Bitboard& bitboard) {
  auto free = bitboard.freeSquares(dy, dx, pov);
  auto take = bitboard.takeSquares(dy, dx, pov);
  uint64_t movable = lastFree & (free | take);
  if (i == 1) {
    movable |= bitboard.getKings() & (free | take);
  }
  lastFree &= lastFree & free;

  return movable;
}

void fillMove(uint64_t movable, int dx, int dy, std::unordered_set<Move, HashMove>& result, uint64_t promotable=0) {
  for (unsigned bitPos = 0; bitPos < 64; bitPos++) {
    uint64_t bitMask = (1ull << bitPos);
    if ((bitMask & movable )!= 0) {
      auto toPos = bitPos-dx+dy*8;
      assert(toPos >= 0);
      assert(toPos < 64);
      if ((bitMask & promotable) != 0) {
        for (char c : {'q','r','n','b'}) {
          result.emplace(bitPos,toPos,c);
        }
      }
      else{
        result.emplace(bitPos,toPos);
      }
    }
  }
}

void queenLikeMoves(const Bitboard& bitboard, bool pov, std::unordered_set<Move, HashMove>& result) {
  auto orthogonalPieces = bitboard.getRooks() | bitboard.getQueens();
  auto diagonalPieces = bitboard.getBishops() | bitboard.getQueens();

  for (int xscale = -1; xscale < 2; ++xscale) {
    for (int yscale = -1; yscale < 2; ++yscale) {
      if (yscale==0 && xscale==0)
        continue;

      bool isPovDirection = yscale == (pov ? 1 : -1);
      bool orthogonal = yscale == 0 || xscale == 0;
      auto majorPieces = orthogonal ? orthogonalPieces : diagonalPieces;
      uint64_t lastFree = majorPieces;

      for (int i = 1; i < 8; i++) {
        auto dx = xscale*i;
        auto dy = yscale*i;
        uint64_t movable = getMovablesQueenLike(dx,dy,i,pov, isPovDirection,lastFree,bitboard);
        fillMove(movable, dx, dy, result);
      }
    }
  }
}

void pawnMoves(const Bitboard& bitboard, bool pov, std::unordered_set<Move, HashMove>& result) {
  int ydir = pov ? 1 : -1;
  uint64_t promotableMask = pov ? 0xffull<<6u*8u : 0xff00ull;
  uint64_t startPosMask = !pov ? 0xffull<<6u*8u : 0xff00ull;
  uint64_t promotablePieces = bitboard.getPawns() & promotableMask;

  uint64_t movable1 = bitboard.getPawns() & bitboard.freeSquares(ydir,0,pov);
  uint64_t movable1right = bitboard.getPawns() & bitboard.takeSquares(ydir, 1, pov);
  uint64_t movable1left = bitboard.getPawns() & bitboard.takeSquares(ydir, -1, pov);

  uint64_t movable2 = movable1 & startPosMask & bitboard.freeSquares(ydir*2,0,pov);

  fillMove(movable1, 0, ydir, result, promotablePieces);
  fillMove(movable1right, 1, ydir, result, promotablePieces);
  fillMove(movable1left, -1, ydir, result, promotablePieces);
  fillMove(movable2, 0, 2*ydir, result);
}

void knightMoves(const Bitboard& bitboard, bool pov, std::unordered_set<Move, HashMove>& result) {
  int dxArray[] = {-2,-2,-1,-1, 1, 1, 2, 2};
  int dyArray[] = {-1, 1,-2, 2,-2, 2,-1, 1};

  for (unsigned idx = 0; idx < 8; idx++) {
    int dx = dxArray[idx];
    int dy = dyArray[idx];
    uint64_t movable = (bitboard.freeSquares(dy,dx,pov) | bitboard.takeSquares(dy,dx,pov)) & bitboard.getKnights();
    fillMove(movable,dx,dy,result);
  }
}

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
  queenLikeMoves(bitboard, pov, result);
  pawnMoves(bitboard, pov, result);
  knightMoves(bitboard, pov, result);
  return result;
}
}
