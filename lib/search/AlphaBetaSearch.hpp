#pragma once

#include <stack>
#include <eval/PieceCountEvaluator.hpp>
#include "Search.hpp"

namespace chess {
class AlphaBetaSearch : Search {
public:
    Move findNextMove(const State &state, const Clock &clock) override;
    AlphaBetaSearch(Evaluator& evaluator) : Search(evaluator) {};
private:
    Score search(const State &state, unsigned depth, bool max, std::optional<Score> alpha, std::optional<Score> beta, std::stack<Move>& line);
};
struct presortingLessThen {
public:
    PieceCountEvaluator evaluator;
    bool maximize;
    explicit presortingLessThen(bool maximize) : evaluator(), maximize(maximize) {};
    inline bool operator() (const std::pair<Move,State>&,const std::pair<Move,State>&);
};
}
