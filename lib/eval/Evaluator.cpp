#include "Evaluator.hpp"

#include <bit>
namespace chess {
    Score Evaluator::operator()(State &state) {
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
