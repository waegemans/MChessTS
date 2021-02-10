#pragma once

#include <vector>
#include <eval/PieceCountEvaluator.hpp>
#include "Search.hpp"
#include "Move.hpp"
#include <atomic>

namespace chess {
class AlphaBetaSearch : public Search {
public:
    Move findNextMove(const State &state, const Clock &clock) override;
    AlphaBetaSearch(Evaluator& evaluator) : Search(evaluator) {};
private:
    static void iterativeDeepeningSearch(const State& state,const Evaluator& evaluator, Move& bestMove, std::atomic<bool>& stop);
    static Score search(const State &state, unsigned maxDepth, bool max, std::optional<Score> alpha, std::optional<Score> beta, std::vector<Move>& line, std::vector<Move>::const_iterator pvBegin, std::vector<Move>::const_iterator pvEnd, const Evaluator& evaluator, std::atomic<bool>& stop);
};
struct presortingLessThen {
public:
    PieceCountEvaluator evaluator;
    bool maximize;
    std::optional<Move> pvMove;
    explicit presortingLessThen(bool maximize, std::optional<Move> pvMove = std::nullopt) : evaluator(), maximize(maximize), pvMove(pvMove) {};
    inline bool operator() (const std::pair<Move,State>&,const std::pair<Move,State>&);
};
}
