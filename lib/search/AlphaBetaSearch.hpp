#pragma once

#include <vector>
#include "Search.hpp"
#include "Move.hpp"
#include <atomic>
#include <eval/PiecePositionEvaluator.hpp>

namespace chess {
class AlphaBetaSearch : public Search {
public:
    Move findNextMove(State &state, const Clock &clock) override;
    AlphaBetaSearch(Evaluator& evaluator) : Search(evaluator) {};
private:
    static void iterativeDeepeningSearch(State& state,const Evaluator& evaluator, Move& bestMove, const Clock& clock);
    static Score search(State &state, unsigned maxDepth, bool max, std::optional<Score> alpha, std::optional<Score> beta, std::vector<Move>& line, std::vector<Move>::const_iterator pvBegin, std::vector<Move>::const_iterator pvEnd, const Evaluator& evaluator, uint64_t& nodes);
};
struct presortingLessThen {
public:
    PiecePositionEvaluator evaluator;
    bool maximize;
    explicit presortingLessThen(bool maximize) : evaluator(), maximize(maximize) {};
    inline bool operator() (const Bitboard&,const Bitboard&);
};
}
