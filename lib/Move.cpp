#include "Move.hpp"

#include <cassert>
#include <functional>

namespace chess {
namespace internal {
char fileCharFromSquare(unsigned square) {
  assert(square >= 0);
  assert(square < 64);
  return 'h' - (square%8);
}
char rowCharFromSquare(unsigned square) {
  assert(square >= 0);
  assert(square < 64);
  return '1' + (square/8);

}
unsigned indexFromFileChar(char file) {
  assert(file >= 'a');
  assert(file <= 'h');
  return 'h' - file;
}

unsigned indexFromRowChar(char row) {
  assert(row >= '1');
  assert(row <= '8');
  return row - '1';
}
}

Move::Move(std::string_view uci) {
  assert(uci.length() == 4 or uci.length() == 5);
  char fromFile = uci[0];
  char fromRow = uci[1];
  char toFile = uci[2];
  char toRow = uci[3];

  fromSquare =       internal::indexFromFileChar(fromFile)
               + 8 * internal::indexFromRowChar(fromRow);

  toSquare =       internal::indexFromFileChar(toFile)
             + 8 * internal::indexFromRowChar(toRow);

  if (uci.length() == 5) {
    promotion = uci[4];
  }
}

std::string Move::toUCI() const {
  return {
      internal::fileCharFromSquare(fromSquare),
      internal::rowCharFromSquare(fromSquare),
      internal::fileCharFromSquare(toSquare),
      internal::rowCharFromSquare(toSquare)
  };
}

bool operator== (const Move& lhs, const Move& rhs) {
  return lhs.promotion == rhs.promotion
    && lhs.fromSquare == rhs.fromSquare
    && lhs.toSquare == rhs.toSquare;
}

size_t HashMove::operator()(const Move& move) const {
  auto uHasher = std::hash<uint64_t>();
  auto oHasher = std::hash<std::optional<char>>();
  auto hashValue = uHasher(move.toSquare);
  hashValue ^= uHasher(move.fromSquare) + 0x9e3779b9 + (hashValue<<6) + (hashValue>>2);
  hashValue ^= oHasher(move.promotion)  + 0x9e3779b9 + (hashValue<<6) + (hashValue>>2);

  return hashValue;
}
}