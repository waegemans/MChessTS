#include "Evaluator.hpp"

#include <bit>
namespace chess {
    Score Evaluator::operator()(const State &state) const {
        if (state.isGameOver()) {
            if (state.isCheck()) {
                return Score(true, state.getPov() ? -1 : 1);
            } else {
                return Score(0);
            }
        }
        return this->evalNotGameOver(state);
    }
}
