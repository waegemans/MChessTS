#pragma once

#include "Evaluator.hpp"

namespace chess {
class PieceCountEvaluator : public Evaluator {
    Score evalNotGameOver(const State &state) const override;
};
}