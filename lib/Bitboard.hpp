#pragma  once

#include <cstdint>
#include <string>
#include <string_view>
#include <bitset>
#include <vector>
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
        mutable uint64_t pinnedHorizontal;
        mutable uint64_t pinnedVertical;
        mutable uint64_t pinnedDiagonal;
        mutable uint64_t pinnedAntidiagonal;
        mutable uint64_t pinnedEnPassant;
        mutable uint64_t checks;
        mutable bool cachedAttack;

        mutable std::optional<std::vector<Move>> legalMovesCache;

    private:
        void evalPawnAttack() const;

        void evalQueenLikeAttack() const;

        void evalQueenLikeAttackXray(int dleft, int dup) const;

        void evalKingAttack() const;

        void evalKnightAttack() const;

        void evalEnPassantPin() const;

        uint64_t &relevantPinMap(int dx, int dy) const;
        uint64_t pinnedAny() const;
        uint64_t pinnedForDirection(int dx, int dy) const;

        void resetCachedAttack() const;


    public:
        [[nodiscard]] uint64_t getKings() const { return kings; };

        [[nodiscard]] uint64_t getQueens() const { return queens; };

        [[nodiscard]] uint64_t getRooks() const { return rooks; };

        [[nodiscard]] uint64_t getKnights() const { return knights; };

        [[nodiscard]] uint64_t getBishops() const { return bishops; };

        [[nodiscard]] uint64_t getPawns() const { return pawns; };

        [[nodiscard]] bool getPov() const { return pov; };

        [[nodiscard]] const std::bitset<4> &getCastlingRights() const { return castlingRights; };

        [[nodiscard]] const std::optional<unsigned int> &getEnPassantFile() const { return enPassantFile; };

        [[nodiscard]] uint64_t getOccupied(bool color) const { return color ? occupiedWhite : occupiedBlack; };

        [[nodiscard]] uint64_t occupied() const { return occupiedWhite | occupiedBlack; };

        [[nodiscard]] uint64_t getControlled() const { return controlled; };

        unsigned int getMoveCounter() const { return moveCounter; };

        unsigned int getHalfMoveCounter() const { return halfMoveCounter; };

        uint64_t getPinnedHorizontal() const { return pinnedHorizontal; };

        uint64_t getPinnedVertical() const { return pinnedVertical; };

        uint64_t getPinnedDiagonal() const { return pinnedDiagonal; };

        uint64_t getPinnedAntidiagonal() const { return pinnedAntidiagonal; };

        bool isCachedAttack() const { return cachedAttack; };

        [[nodiscard]] uint64_t getChecks() const { return checks; };

        void evalAttack() const;

        void startpos();

        void parseFEN(std::string_view fen);

        std::vector<Move> legalMoves() const;

    private:
        void kingMoves(std::vector<Move> &result) const;

        uint64_t getCheckBlockCaptureSquares() const;

        void queenLikeMoves(std::vector<Move> &result, uint64_t targetSquares) const;
        void queenLikeMovesSingleRay(std::vector<Move> &result, uint64_t targetSquares, int dx, int dy) const;

        void knightMoves(std::vector<Move> &result, uint64_t targetSquares) const;

        void pawnMoves(std::vector<Move> &result, uint64_t targetSquares) const;

        void castlingMoves(std::vector<Move> &result) const;

    private:
        void parseBoardFEN(std::string_view boardFen);

        void parsePovFEN(std::string_view povFen);

        void parseCastlingFEN(std::string_view castlingFen);

        void parseEnPassantFEN(std::string_view enPassantFen);

        void parseHalfMoveFEN(std::string_view halfMoveFen);

    public:
        std::string to_fen() const;

        [[nodiscard]] std::string to_string() const;


        void applyMoveSelf(const Move &move);

        Bitboard applyMoveCopy(const Move &move) const;

    private:
        void preApplyPromotion(uint64_t fromMask, const Move &move);

        void preApplyCastling(const Move &move);

        void preApplyToggleEnPassant(const Move &move);

        void preApplyEnPassantCapture(unsigned toSquare);

        void preApplyRemoveCastlingKingMove();

        void preApplyRemoveCastlingRook(const Move &move);

        void movePiece(unsigned fromSquare, unsigned toSquare);

        void evalEnPassantLegality();

    public:
        bool operator==(const Bitboard &other) const;

        static constexpr uint64_t canMoveToMask(int left, int up);

        static constexpr uint64_t applyOffset(int left, int up, uint64_t board);

        static void appendMoves(std::vector<Move>& result, uint64_t movablePieces, int dx, int dy, uint64_t promotable = 0);

        bool isGameOver() const;

        bool isCheck() const;

        bool isDraw50() const;

        bool isDrawInsufficient() const;
    }; // class Bitboard
} // namespace chess
