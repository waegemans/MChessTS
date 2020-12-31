#pragma once

#include <State.hpp>
#include <eval/Score.hpp>

namespace chess {

class Evaluator {
private:
    virtual Score evalNotGameOver(State&) = 0;
public:
    Score operator()(State&);
};

} // namespace chess
