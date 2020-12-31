#pragma once

#include <string>
#include <ostream>

namespace chess {
class Score {
public:
    bool isMate;
    int value;

    Score(bool isMate, int value) : isMate(isMate),value(value){};
    explicit Score(int value) : isMate(false),value(value){};

    bool operator< (const Score&) const;
    bool operator> (const Score&) const;
    bool operator<= (const Score&) const;
    bool operator>= (const Score&) const;
    bool operator== (const Score&) const;
    bool operator!= (const Score&) const;
};
} // namespace chess
