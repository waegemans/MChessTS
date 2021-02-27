#include "PieceCountEvaluator.hpp"

#include <bit>

namespace chess {
    Score PieceCountEvaluator::evalNotGameOver(const State &state) const {
        int cpValue = 0;
        const auto& bitboard = state.getBitboard();
        cpValue += 100*std::popcount(bitboard.getPawns() & bitboard.occupiedWhite);
        cpValue += 320*std::popcount(bitboard.getBishops() & bitboard.occupiedWhite);
        cpValue += 300*std::popcount(bitboard.getKnights() & bitboard.occupiedWhite);
        cpValue += 500*std::popcount(bitboard.getRooks() & bitboard.occupiedWhite);
        cpValue += 900*std::popcount(bitboard.getQueens() & bitboard.occupiedWhite);


        cpValue -= 100*std::popcount(bitboard.getPawns() & bitboard.occupiedBlack);
        cpValue -= 320*std::popcount(bitboard.getBishops() & bitboard.occupiedBlack);
        cpValue -= 300*std::popcount(bitboard.getKnights() & bitboard.occupiedBlack);
        cpValue -= 500*std::popcount(bitboard.getRooks() & bitboard.occupiedBlack);
        cpValue -= 900*std::popcount(bitboard.getQueens() & bitboard.occupiedBlack);

        return Score(cpValue);
    }
}
