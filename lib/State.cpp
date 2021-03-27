#include "State.hpp"

#include <cassert>

namespace chess {
    void State::reset() {
        stack.clear();
        startingPosition.startpos();
    }

    void State::parseFen(std::string_view fen) {
        stack.clear();
        startingPosition.parseFEN(fen);
    }

    const Bitboard &State::getCurrentBitboard() const {
        return stack.empty() ? startingPosition : stack.back();
    }

    void State::pushMove(Move move) {
        stack.push_back(getCurrentBitboard().applyMoveCopy(move));
    }

    void State::pushBoard(Bitboard&& board) {
        stack.push_back(board);
    }

    void State::popBoard() {
        assert(!stack.empty());
        stack.pop_back();
    }

    bool State::isTreefoldRepetition() const {
        if (stack.empty()) return false;
        auto currentBoard = getCurrentBitboard();
        if (currentBoard.getHalfMoveCounter() < 8) return false;
        unsigned lastHalfMoveCount = currentBoard.getHalfMoveCounter();
        unsigned repetitions = 1;
        for (auto it = stack.crbegin()+1;it != stack.crend(); it++) {
            if (lastHalfMoveCount < it->getHalfMoveCounter()) break;
            if (*it == currentBoard) repetitions += 1;
            if (repetitions == 3) break;
            lastHalfMoveCount = it->getHalfMoveCounter();
        }
        if (currentBoard == startingPosition) repetitions+=1;
        return (repetitions >= 3);
    }

    bool State::isGameOver() const {
        return getCurrentBitboard().isGameOver() | isTreefoldRepetition();
    }
}
