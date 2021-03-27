#pragma once

#include "Evaluator.hpp"

namespace chess {
    class PiecePositionEvaluator : public Evaluator {
        Score evalNotGameOver(const State &state) const override;

    public:
        Score evalNotGameOver(const Bitboard& bitboard) const;
    };
}