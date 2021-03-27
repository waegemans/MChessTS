
#include "AlphaBetaSearch.hpp"

#include <cassert>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

namespace chess {
    Move AlphaBetaSearch::findNextMove(State &state, const Clock &clock) {
        Move bestMove{0,0};
        auto worker = std::thread(iterativeDeepeningSearch,std::ref(state),std::ref(evaluator), std::ref(bestMove),std::ref(clock));
        worker.join();
        return bestMove;
    }

    void AlphaBetaSearch::iterativeDeepeningSearch(State& state, const Evaluator& evaluator, Move& bestMove, const Clock& clock) {
        std::vector<Move> pv;
        unsigned depth;
        uint64_t nodes = 0;
        auto startTime = std::chrono::high_resolution_clock::now();
        auto increment = std::chrono::milliseconds(state.getCurrentBitboard().getPov() ? clock.white_increment_ms : clock.black_increment_ms);
        for (depth = 1; depth < 6 && (std::chrono::high_resolution_clock::now() < (startTime+increment)) ; depth++) {
            std::vector<Move> line;
            auto score = search(state, depth, state.getCurrentBitboard().getPov(), std::nullopt, std::nullopt, line, pv.cbegin(), pv.cend(), evaluator, nodes);
            pv = std::move(line);
            bestMove = pv.front();
            if (score.isMate) {
                break;
            }
            std::cerr << "info nodes " << nodes << std::endl;
            std::cerr << "info depth " << depth << std::endl;
        }
    }

    Score AlphaBetaSearch::search(State &state, unsigned maxDepth, bool max, std::optional<Score> alpha,
                                  std::optional<Score> beta, std::vector<Move> &line, std::vector<Move>::const_iterator pvBegin, std::vector<Move>::const_iterator pvEnd,const Evaluator& evaluator, uint64_t& nodes) {
        if (maxDepth == 0 || state.isGameOver()) {
            nodes++;
            return evaluator(state);
        }
        std::optional<Score> bestScore;
        std::vector<Bitboard> nextBoards;
        std::vector<Move> moves = state.getCurrentBitboard().legalMoves();
        nextBoards.reserve(moves.size());
        for (auto move : moves) {
            nextBoards.push_back(state.getCurrentBitboard().applyMoveCopy(move));
        }
        auto sortFunction = presortingLessThen(state.getCurrentBitboard().getPov());
        for (auto it = nextBoards.begin(); it != nextBoards.end(); it++) {
            auto minIt = std::min_element(it, nextBoards.end(), sortFunction);
            auto idx = it - nextBoards.begin();
            auto minIdx = minIt - nextBoards.begin();
            std::iter_swap(it, minIt);
            std::iter_swap(moves.begin()+idx, moves.begin()+minIdx);
            Bitboard board = std::move(*it);
            Move move = *(moves.begin()+idx);



            state.pushBoard(std::move(board));
            std::vector<Move> nextLine;
            auto nextScore = search(state, maxDepth - 1, !max, alpha, beta, nextLine, pvBegin, pvEnd,evaluator,nodes);
            state.popBoard();
            // update new optimum
            if (!bestScore || (max ? nextScore > bestScore.value() : nextScore < bestScore.value())) {
                bestScore = nextScore;
                line = std::move(nextLine);
                line.insert(line.begin(), move);
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


    bool presortingLessThen::operator()(const Bitboard &lhs, const Bitboard &rhs) {
        bool leftCheck = lhs.isCheck();
        bool rightCheck = rhs.isCheck();

        if (leftCheck ^ rightCheck) return leftCheck;
        auto leftEval = evaluator.evalNotGameOver(lhs);
        auto rightEval = evaluator.evalNotGameOver(rhs);
        return maximize ? leftEval > rightEval : leftEval < rightEval;
    }
}