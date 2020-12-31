#include "Score.hpp"

#include <cassert>

namespace chess {
    bool Score::operator< (const Score& other) const {
        // both cp
        if (!isMate && !other.isMate) return value < other.value;
        // both mate
        if (isMate && other.isMate) {
            assert(value != 0);
            assert(other.value != 0);
            // different signs
            if ((value < 0)^(other.value < 0)) return value < other.value;
            return value > other.value;
        }
        // only this mate
        if (isMate) return (value<0);
        // only other mate
        return (other.value>0);

    }

    bool Score::operator>(const Score &other) const {
        return other < *this;
    }

    bool Score::operator<=(const Score &other) const {
        return !(*this > other);
    }

    bool Score::operator>=(const Score &other) const {
        return !(*this < other);
    }

    bool Score::operator==(const Score &other) const {
        return (*this <= other) && (*this >= other);
    }

    bool Score::operator!=(const Score &other) const {
        return !(*this == other);
    }
}

