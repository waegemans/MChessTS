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
  std::optional<unsigned> enPassantFile;

  unsigned halfMoveCount;
  unsigned fullMoveCount;

  void parseCastlingFen(std::string_view);
  void parseEnPassantFen(std::string_view en_passant_fen);
  [[nodiscard]] bool isLegal(bool flipPov = false) const;

public:
  void reset ();
  void parseFen(std::string_view);
  [[nodiscard]] const Bitboard& getBitboard() const { return bitboard; };
  [[nodiscard]] bool getPov() const { return pov;};
  [[nodiscard]] std::bitset<4> getCastlingRights() const { return castlingRights;};
  [[nodiscard]] std::optional<unsigned> getEnPassantFile() const { return enPassantFile;};
  [[nodiscard]] std::unordered_set<Move, HashMove> pseudoLegalMoves(bool flipPov = false) const;
  [[nodiscard]] std::unordered_set<Move, HashMove> legalMoves() const;
  [[nodiscard]] State applyMove(Move) const;
  void applyMoveSelf(Move move);

    bool isCheck() const;
    bool isGameOver() const;
    bool isInsufficient() const;
};
}
