#pragma once

#include <State.hpp>
#include <eval/Score.hpp>

namespace chess {

class Evaluator {
private:
    [[nodiscard]] virtual Score evalNotGameOver(const State&) const = 0;
public:
    Score operator()(const State&) const;
};

} // namespace chess
