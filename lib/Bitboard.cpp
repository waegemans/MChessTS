#include "Bitboard.hpp"

#include <cassert>
#include <cstring>
#include <stdexcept>

namespace chess {
    void Bitboard::parse_fen(std::string_view boardFen) {
        kings = queens = rooks = knights = bishops = pawns = occupied_white = occupied_black = 0;
        uint64_t current = 1;
        current <<= 63u;
        for (char c : boardFen) {
            assert(current != 0);
            // skip dashes
            if (c == '/') {
                continue;
            }
            // move number of empty spaces
            if (std::isdigit(c)) {
                current >>= static_cast<uint>(c - '0');
                continue;
            }

            // mark as white or black
            if (std::isupper(c)) {
                occupied_white |= current;
            } else {
                occupied_black |= current;
            }

            char piece = static_cast<char>(std::tolower(c));

            switch (piece) {
                case 'k':
                    kings |= current;
                    break;
                case 'q':
                    queens |= current;
                    break;
                case 'r':
                    rooks |= current;
                    break;
                case 'n':
                    knights |= current;
                    break;
                case 'b':
                    bishops |= current;
                    break;
                case 'p':
                    pawns |= current;
                    break;
                default:
                    assert(false);
            }
            current >>= 1u;
        }
        assert(current == 0);
    }

    std::string Bitboard::to_fen() { throw std::logic_error("Not implemented!"); }

    std::string Bitboard::to_string() const {
        std::string retVal(8 * 9, '.');
        uint64_t currentidx = 1;
        currentidx <<= 63u;
        for (uint i = 0; i < retVal.size(); i++) {
            if (i % 9 == 8) {
                retVal[i] = '\n';
                continue;
            }
            if (((occupied_black | occupied_white) & currentidx) != 0) {
                bool white = (occupied_white & currentidx) != 0;
                if ((kings & currentidx) != 0) retVal[i] = white ? 'K' : 'k';
                if ((queens & currentidx) != 0) retVal[i] = white ? 'Q' : 'q';
                if ((rooks & currentidx) != 0) retVal[i] = white ? 'R' : 'r';
                if ((knights & currentidx) != 0) retVal[i] = white ? 'N' : 'n';
                if ((bishops & currentidx) != 0) retVal[i] = white ? 'B' : 'b';
                if ((pawns & currentidx) != 0) retVal[i] = white ? 'P' : 'p';
            }
            currentidx >>= 1u;
        }

        return retVal;
    }

    uint64_t Bitboard::checkSquares(int up, int right, bool pov, uint64_t toMask) const {
        assert(up < 8);
        assert(up > -8);
        assert(right < 8);
        assert(right > -8);

        uint64_t occupied_own = occupied_pov(pov);
        int offset = up * 8 - right;
        uint64_t rowMask = 0;
        uint8_t bitmask = ~static_cast<uint8_t>(0);
        if (right > 0) {
            bitmask <<= static_cast<unsigned>(right);
        } else {
            bitmask >>= static_cast<unsigned >(-right);
        }
        std::memset(&rowMask, bitmask, 8);
        occupied_own &= rowMask;
        bool isNegativeOffset = offset < 0;
        unsigned absOffset = isNegativeOffset ? -offset : offset;
        if (isNegativeOffset) return occupied_own & (toMask << absOffset);
        return occupied_own & (toMask >> absOffset);

    }

    uint64_t Bitboard::freeSquares(int up, int right, bool pov) const {
        auto notOccupied = ~(occupied_white | occupied_black);
        return checkSquares(up, right, pov, notOccupied);
    }

    uint64_t Bitboard::takeSquares(int up, int right, bool pov) const {
        return checkSquares(up, right, pov, getOccupiedPov(!pov));

    }

    uint64_t Bitboard::occupied_pov(bool pov) const {
        return pov ? occupied_white : occupied_black;
    }

    uint64_t Bitboard::occupied() const {
        return occupied_black | occupied_white;
    }

    void Bitboard::startpos() {
        kings = (0x08ull << 7u * 8u) | (0x08ull);
        queens = kings << 1u;
        bishops = kings >> 1u | queens << 1u;
        knights = kings >> 2u | queens << 2u;
        rooks = kings >> 3u | queens << 3u;
        pawns = (0xffull << 6u * 8u) | (0xff00ull);
        occupied_white = 0xffffull;
        occupied_black = occupied_white << 6u * 8u;
    }

    void Bitboard::applyMove(unsigned int fromSquare, unsigned int toSquare) {
        uint64_t fromMask = 1ull<<fromSquare;
        uint64_t toMask = 1ull<<toSquare;

        uint64_t otherMask = ~(fromMask|toMask);

        kings = (kings & otherMask) | ( (kings&fromMask) !=0)*toMask;
        queens = (queens & otherMask) | ( (queens&fromMask) !=0)*toMask;
        bishops = (bishops & otherMask) | ( (bishops&fromMask) !=0)*toMask;
        knights = (knights & otherMask) | ( (knights&fromMask) !=0)*toMask;
        rooks = (rooks & otherMask) | ( (rooks&fromMask) !=0)*toMask;
        pawns = (pawns & otherMask) | ( (pawns&fromMask) !=0)*toMask;
        occupied_white = (occupied_white & otherMask) | ( (occupied_white&fromMask) !=0)*toMask;
        occupied_black = (occupied_black & otherMask) | ( (occupied_black&fromMask) !=0)*toMask;
    }

} // namespace chess