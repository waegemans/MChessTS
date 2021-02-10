#include <bit>
#include "PiecePositionEvaluator.hpp"

constexpr uint64_t mainDiagonal = 0x8142241818244281ull;
constexpr uint64_t offDiagonal = 0x42a55a3c3c5aa542ull;
constexpr uint64_t center = 0x0000001818000000ull;
constexpr uint64_t offCenter = 0x0000182424180000ull;
namespace chess {
    Score PiecePositionEvaluator::evalNotGameOver(const State &state) const {
        int cpValue = 0;
        const auto& bitboard = state.getBitboard();
        for (bool pov : {true,false}){
            int povFactor = (pov ? 1 : -1 );
            uint64_t occupied = bitboard.getOccupiedPov(pov);

            cpValue += povFactor*100*std::popcount(bitboard.getPawns() & occupied);
            cpValue += povFactor*300*std::popcount(bitboard.getBishops() & occupied);
            cpValue += povFactor*300*std::popcount(bitboard.getKnights() & occupied);
            cpValue += povFactor*480*std::popcount(bitboard.getRooks() & occupied);
            cpValue += povFactor*900*std::popcount(bitboard.getQueens() & occupied);
            //bishop diagonal
            cpValue += povFactor*10*std::popcount(bitboard.getBishops() & occupied & mainDiagonal);
            cpValue += povFactor*7*std::popcount(bitboard.getBishops() & occupied & offDiagonal);
            //knight center
            cpValue += povFactor*10*std::popcount(bitboard.getKnights() & occupied & center);
            cpValue += povFactor*2*std::popcount(bitboard.getKnights() & occupied & offCenter);
            //pawn center
            cpValue += povFactor*3*std::popcount(bitboard.getPawns() & occupied & center);
        }

        return Score(cpValue);
    }
}
