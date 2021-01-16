#pragma once

#include <Move.hpp>
#include <State.hpp>
#include <Clock.hpp>
#include <eval/Evaluator.hpp>

namespace chess{
class Search {
protected:
    Evaluator& evaluator;
public:
    virtual Move findNextMove(const State& state,const Clock& clock) = 0;
    Search(Evaluator& evaluator) : evaluator(evaluator) {};
};
}
