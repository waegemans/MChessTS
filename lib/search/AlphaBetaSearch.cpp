
#include "AlphaBetaSearch.hpp"

#include <cassert>
#include <iostream>
#include <vector>

namespace chess {
    Move AlphaBetaSearch::findNextMove(const State &state, const Clock &clock) {
        std::stack<Move> line;
        for (unsigned depth = 1; clock.white_time_ms == 0 && depth < 10; depth++) {
            auto score = search(state, depth, state.getPov(), std::nullopt, std::nullopt, line);
            if (score.isMate) {
                break;
            }
        }
        return line.top();
    }

    Score AlphaBetaSearch::search(const State &state, unsigned depth, bool max, std::optional<Score> alpha,
                                  std::optional<Score> beta, std::stack<Move> &line) {
        if (depth == 0 || state.isGameOver()) {
            return Search::evaluator(state);
        }
        std::optional<Score> bestScore;
        std::vector<std::pair<Move, State>> nextMoveStateVector{};
        for (const auto &move : state.legalMoves()) {
            nextMoveStateVector.emplace_back(move, state.applyMove(move));
        }
        std::sort(nextMoveStateVector.begin(), nextMoveStateVector.end(), presortingLessThen(max));
        for (const auto&[move, nextState] : nextMoveStateVector) {
            std::stack<Move> nextLine;
            auto nextScore = search(nextState, depth - 1, !max, alpha, beta, nextLine);
            // update new optimum
            if (!bestScore || (max ? nextScore > bestScore.value() : nextScore < bestScore.value())) {
                line = nextLine;
                line.push(move);
                bestScore = nextScore;
            }
            // alpha pruning
            if (!max && alpha.has_value() && bestScore.value() < alpha.value()) {
                break;
            }
            // beta pruning
            if (max && beta.has_value() && bestScore.value() > beta.value()) {
                break;
            }
            // mate pruning
            if (max && bestScore.value().isMate && bestScore.value().value > 0) {
                break;
            }
            if (!max && bestScore.value().isMate && bestScore.value().value < 0) {
                break;
            }
            // update alpha and beta
            if (max && (!alpha.has_value() || bestScore.value() > alpha.value())) {
                alpha = bestScore;
            }
            if (!max && (!beta.has_value() || bestScore.value() < beta.value())) {
                beta = bestScore;
            }
        }
        assert(bestScore);
        return bestScore.value();
    }


    bool presortingLessThen::operator()(const std::pair<Move, State> &lhs, const std::pair<Move, State> &rhs) {
        bool leftCheck = lhs.second.isCheck();
        bool rightCheck = rhs.second.isCheck();

        if (leftCheck ^ rightCheck) return leftCheck;
        auto leftEval = evaluator.evalNotGameOver(lhs.second);
        auto rightEval = evaluator.evalNotGameOver(rhs.second);
        return maximize ? leftEval > rightEval : leftEval < rightEval;
    }
}