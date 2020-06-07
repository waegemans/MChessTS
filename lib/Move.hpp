#pragma once

#include <cstdint>
#include <optional>

namespace chess {
class Move {
public:
  unsigned fromSquare;
  unsigned toSquare;
  std::optional<char> promotion;

public:
  Move(unsigned fromSquare, unsigned toSquare) : fromSquare(fromSquare), toSquare(toSquare) {};
  explicit Move(std::string_view uci);
  [[nodiscard]] std::string toUCI() const;
};

bool operator== (const Move&, const Move&);

struct HashMove {
  size_t operator() (const Move&) const;
};
}
