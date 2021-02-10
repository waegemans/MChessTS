#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace chess {
class Move {
public:
  unsigned fromSquare;
  unsigned toSquare;
  std::optional<char> promotion;

public:
  Move(unsigned fromSquare, unsigned toSquare) : fromSquare(fromSquare), toSquare(toSquare) {};
  Move(unsigned fromSquare, unsigned toSquare, char promotion) : fromSquare(fromSquare), toSquare(toSquare), promotion(promotion) {};
  explicit Move(std::string_view uci);
  [[nodiscard]] std::string toUCI() const;
  [[nodiscard]] unsigned fileDistance() const;
  [[nodiscard]] unsigned rowDistance() const;
};

bool operator== (const Move&, const Move&);

struct HashMove {
  size_t operator() (const Move&) const;
};
}
