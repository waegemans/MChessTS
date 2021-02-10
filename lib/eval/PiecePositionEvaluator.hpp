#pragma once

#include "Evaluator.hpp"

namespace chess {
    class PiecePositionEvaluator : public Evaluator {
        Score evalNotGameOver(const State &state) const override;
    };
}