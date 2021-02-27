#pragma  once

#include <cstdint>
#include <string>
#include <string_view>
#include <bitset>
#include <cassert>
#include <cstring>
#include <stdexcept>
#include "Move.hpp"

namespace chess {
    class Bitboard {
    private:
        uint64_t kings;
        uint64_t queens;
        uint64_t rooks;
        uint64_t knights;
        uint64_t bishops;
        uint64_t pawns;

        uint64_t occupiedWhite;
        uint64_t occupiedBlack;

        bool pov;
        std::bitset<4> castlingRights;
        std::optional<unsigned> enPassantFile;

        unsigned moveCounter;
        unsigned halfMoveCounter;

        mutable uint64_t controlled;
        mutable uint64_t pinned;
        mutable uint64_t checks;

    private:
        void evalPawnAttack() const;

        void evalQueenLikeAttack() const;

        void evalQueenLikeAttackXray(int dleft, int dup) const;

        void evalKingAttack() const;

        void evalKnightAttack() const;

    public:
        [[nodiscard]] uint64_t getKings() const { return kings; };

        [[nodiscard]] uint64_t getQueens() const { return queens; };

        [[nodiscard]] uint64_t getRooks() const { return rooks; };

        [[nodiscard]] uint64_t getKnights() const { return knights; };

        [[nodiscard]] uint64_t getBishops() const { return bishops; };

        [[nodiscard]] uint64_t getPawns() const { return pawns; };

        bool getPov() const { return pov; };

        const std::bitset<4> &getCastlingRights() const { return castlingRights; };

        const std::optional<unsigned int> &getEnPassantFile() const { return enPassantFile; };

        [[nodiscard]] uint64_t getOccupied(bool color) const { return color ? occupiedWhite : occupiedBlack; };

        [[nodiscard]] uint64_t occupied() const { return occupiedWhite | occupiedBlack; };

        [[nodiscard]] uint64_t getControlled() const { return controlled; };

        [[nodiscard]] uint64_t getPinned() const { return pinned; };

        [[nodiscard]] uint64_t getChecks() const { return checks; };

        void evalAttack() const;

        void startpos();

        void parseFEN(std::string_view fen);

    private:
        void parseBoardFEN(std::string_view boardFen);

        void parsePovFEN(std::string_view povFen);

        void parseCastlingFEN(std::string_view castlingFen);

        void parseEnPassantFEN(std::string_view enPassantFen);

    public:
        std::string to_fen();

        [[nodiscard]] std::string to_string() const;


        void applyMoveSelf(const Move& move);
        Bitboard applyMoveCopy(const Move& move) const;
    private:
        void preApplyPromotion(uint64_t fromMask, const Move& move);
        void preApplyCastling(const Move& move);
        void preApplyToggleEnPassant(const Move& move);
        void preApplyEnPassantCapture(unsigned toSquare);
        void preApplyRemoveCastlingKingMove();
        void preApplyRemoveCastlingRook(const Move& move);
        void movePiece(unsigned fromSquare, unsigned toSquare);
        void evalEnPassantLegality();

    public:
        bool operator==(const Bitboard &other) const;

        static constexpr uint64_t canMoveToMask(int left, int up);

        static constexpr uint64_t applyOffset(int left, int up, uint64_t board);
    }; // class Bitboard
} // namespace chess
