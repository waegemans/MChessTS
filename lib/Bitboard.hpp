#pragma  once

#include <cstdint>
#include <string>
#include <string_view>

namespace chess {
class Bitboard {
public:
  uint64_t kings;
  uint64_t queens;
  uint64_t rooks;
  uint64_t knights;
  uint64_t bishops;
  uint64_t pawns;

  uint64_t occupied_white;
  uint64_t occupied_black;
private:
  [[nodiscard]] uint64_t occupied_pov(bool pov) const;
  [[nodiscard]] uint64_t checkSquares(int up, int right, bool pov, uint64_t toMask) const;

public:
  [[nodiscard]] uint64_t getKings() const {return kings;};
  [[nodiscard]] uint64_t getQueens() const {return queens;};
  [[nodiscard]] uint64_t getRooks() const {return rooks;};
  [[nodiscard]] uint64_t getKnights() const {return knights;};
  [[nodiscard]] uint64_t getBishops() const {return bishops;};
  [[nodiscard]] uint64_t getPawns() const {return pawns;};

  [[nodiscard]] uint64_t getOccupiedPov(bool pov) const {return pov ? occupied_white : occupied_black;};
  [[nodiscard]] uint64_t occupied() const;

  void startpos();
  void parse_fen(std::string_view boardFen);
  std::string to_fen();
  [[nodiscard]] std::string to_string() const;


  [[nodiscard]] uint64_t freeSquares(int up, int right, bool pov) const;
  [[nodiscard]] uint64_t takeSquares(int up, int right, bool pov) const;

  void applyMove(unsigned fromSquare, unsigned toSquare);

  bool operator==(const Bitboard& other) const;

}; // class Bitboard
} // namespace chess
