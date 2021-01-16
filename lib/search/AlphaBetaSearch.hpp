#pragma once

#include <stack>
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
    Evaluator& evaluator;
    explicit presortingLessThen(Evaluator& evaluator) : evaluator(evaluator) {};
    inline bool operator() (const std::pair<Move,State>&,const std::pair<Move,State>&);
};
}
