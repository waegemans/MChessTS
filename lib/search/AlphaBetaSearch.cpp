
#include "AlphaBetaSearch.hpp"

#include <cassert>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

namespace chess {
    Move AlphaBetaSearch::findNextMove(const State &state, const Clock &clock) {
        Move bestMove{0,0};
        std::atomic<bool> stop = false;
        auto worker = std::thread(iterativeDeepeningSearch,std::ref(state),std::ref(evaluator), std::ref(bestMove),std::ref(stop));

        std::this_thread::sleep_for(std::chrono::milliseconds(state.getPov() ? clock.white_increment_ms : clock.black_increment_ms));
        stop = true;
        worker.join();
        return bestMove;
    }

    void AlphaBetaSearch::iterativeDeepeningSearch(const State& state, const Evaluator& evaluator, Move& bestMove, std::atomic<bool>& stop) {
        std::vector<Move> pv;
        unsigned depth;
        for (depth = 1; !stop ; depth++) {
            std::vector<Move> line;
            auto score = search(state, depth, state.getPov(), std::nullopt, std::nullopt, line, pv.cbegin(), pv.cend(), evaluator, stop);
            pv = std::move(line);
            if (!stop || depth==1) bestMove = pv.front();
            if (score.isMate) {
                break;
            }
        }
        std::cerr << "info depth " << depth << std::endl;
    }

    Score AlphaBetaSearch::search(const State &state, unsigned maxDepth, bool max, std::optional<Score> alpha,
                                  std::optional<Score> beta, std::vector<Move> &line, std::vector<Move>::const_iterator pvBegin, std::vector<Move>::const_iterator pvEnd,const Evaluator& evaluator, std::atomic<bool>& stop) {
        if (maxDepth == 0 || state.isGameOver()|| stop) {
            return evaluator(state);
        }
        std::optional<Score> bestScore;
        std::vector<std::pair<Move, State>> nextMoveStateVector{};
        for (const auto &move : state.legalMoves()) {
            nextMoveStateVector.emplace_back(move, state.applyMove(move));
        }
        std::optional<Move> pvMove;
        if (pvBegin != pvEnd) {
            pvMove = *pvBegin;
        }
        //std::sort(nextMoveStateVector.begin(), nextMoveStateVector.end(), presortingLessThen(max,pvMove));
        for (auto it = nextMoveStateVector.begin(); it != nextMoveStateVector.end(); it++) {
            // move best value to front
            std::iter_swap(it, std::min_element(it, nextMoveStateVector.end(),presortingLessThen(max,pvMove)));
            auto [move,nextState] = *it;
            std::vector<Move> nextLine;
            auto nextPvBegin = pvEnd;
            if (pvBegin != pvEnd && move == *pvBegin) {
                nextPvBegin = pvBegin+1;
            }
            auto nextScore = search(nextState, maxDepth - 1, !max, alpha, beta, nextLine, nextPvBegin, pvEnd,evaluator,stop);
            // update new optimum
            if (!bestScore || (max ? nextScore > bestScore.value() : nextScore < bestScore.value())) {
                line = std::move(nextLine);
                line.insert(line.begin(),move);
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
        if(pvMove.has_value()) {
            if (lhs.first == pvMove.value()) return true;
            if (rhs.first == pvMove.value()) return false;
        }
        bool leftCheck = lhs.second.isCheck();
        bool rightCheck = rhs.second.isCheck();

        if (leftCheck ^ rightCheck) return leftCheck;
        auto leftEval = evaluator.evalNotGameOver(lhs.second);
        auto rightEval = evaluator.evalNotGameOver(rhs.second);
        return maximize ? leftEval > rightEval : leftEval < rightEval;
    }
}