#include "PieceCountEvaluator.hpp"

#include <bit>

namespace chess {
    Score PieceCountEvaluator::evalNotGameOver(const State &state) const {
        int cpValue = 0;
        const auto& bitboard = state.getBitboard();
        cpValue += 100*std::popcount(bitboard.getPawns() & bitboard.occupied_white);
        cpValue += 320*std::popcount(bitboard.getBishops() & bitboard.occupied_white);
        cpValue += 300*std::popcount(bitboard.getKnights() & bitboard.occupied_white);
        cpValue += 500*std::popcount(bitboard.getRooks() & bitboard.occupied_white);
        cpValue += 900*std::popcount(bitboard.getQueens() & bitboard.occupied_white);


        cpValue -= 100*std::popcount(bitboard.getPawns() & bitboard.occupied_black);
        cpValue -= 320*std::popcount(bitboard.getBishops() & bitboard.occupied_black);
        cpValue -= 300*std::popcount(bitboard.getKnights() & bitboard.occupied_black);
        cpValue -= 500*std::popcount(bitboard.getRooks() & bitboard.occupied_black);
        cpValue -= 900*std::popcount(bitboard.getQueens() & bitboard.occupied_black);

        return Score(cpValue);
    }
}
