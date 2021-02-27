#include "Bitboard.hpp"

#include <cassert>
#include <cstring>
#include <stdexcept>

namespace chess {
    namespace {
        constexpr void assertOffset(int left, int up) {
            assert(left < 8);
            assert(left > -8);
            assert(up < 8);
            assert(up > -8);
            assert(up != 0 or left != 0);
        }
    }

    void Bitboard::evalAttack() const {
        if (cachedAttack)
            return;
        evalQueenLikeAttack();
        evalKnightAttack();
        evalPawnAttack();
        evalKingAttack();
        cachedAttack = true;
    }

    void Bitboard::resetCachedAttack() const {
        pinnedHorizontal = 0;
        pinnedVertical = 0;
        pinnedDiagonal = 0;
        pinnedAntidiagonal = 0;
        controlled = 0;
        checks = 0;
        cachedAttack = false;
    }

    void Bitboard::evalPawnAttack() const {
        int up = pov ? -1 : 1;
        uint64_t enemyPawns = getOccupied(!pov) & pawns;
        uint64_t ownKing = getOccupied(pov) & kings;
        for (int left : {-1, 1}) {
            uint64_t pawnControlled = applyOffset(left, up, enemyPawns & canMoveToMask(left, up));
            controlled |= pawnControlled;
            // pawn attacking king
            if (pawnControlled & ownKing) {
                checks |= applyOffset(-left, -up, ownKing);
            }
        }
    }

    void Bitboard::evalQueenLikeAttack() const {
        for (int dleft : {-1, 0, 1}) {
            for (int dup : {-1, 0, 1}) {
                if (dleft == 0 && dup == 0) continue;
                evalQueenLikeAttackXray(dleft, dup);
            }
        }
    }

    void Bitboard::evalQueenLikeAttackXray(int dleft, int dup) const {
        assert(dleft < 2 && dleft > -2);
        assert(dup < 2 && dup > -2);
        assert(dleft != 0 || dup != 0);

        uint64_t lastFree = ~0ull;
        uint64_t xRayActive = 0;
        uint64_t xRayLevel[8] = {0};

        uint64_t ownKing = getOccupied(pov) & kings;
        uint64_t ownNonKing = getOccupied(pov) & (~ownKing);
        uint64_t pinAttacker = 0;

        uint64_t relevantEnemyPieces = (queens | ((dleft == 0 || dup == 0) ? rooks : bishops)) & getOccupied(!pov);
        for (int step = 1; step < 8; step++) {
            int left = dleft * step;
            int up = dup * step;

            uint64_t enemyControlled = applyOffset(left, up, relevantEnemyPieces & lastFree & canMoveToMask(left, up));
            uint64_t enemyControlledPieces = enemyControlled & ownNonKing;
            uint64_t enemyXrayControlled = applyOffset(left, up,
                                                       relevantEnemyPieces & xRayActive & canMoveToMask(left, up));
            uint64_t enemyXrayContolledOccupied = enemyXrayControlled & occupied();

            // update last free
            lastFree &= applyOffset(-left, -up, enemyControlled & (~occupied() | ownKing));
            // mark piece that is the reason for a pin
            pinAttacker |= applyOffset(-left, -up, enemyXrayControlled & ownKing);
            // deactivate xray on second collision
            xRayActive &= ~applyOffset(-left, -up, enemyXrayContolledOccupied);
            // activate xray on controlled pieces
            xRayLevel[step] = applyOffset(-left, -up, enemyControlledPieces);
            xRayActive |= xRayLevel[step];
            // mark checks where enemy attacks king
            checks |= applyOffset(-left, -up, enemyControlled & ownKing);
            controlled |= enemyControlled;
        }
        if (pinAttacker) {
            for (int step = 1; step < 8; step++) {
                relevantPinMap(dleft, dup) |= applyOffset(step * dleft, step * dup, xRayLevel[step] & pinAttacker);
            }
        }

    }

    void Bitboard::evalKingAttack() const {
        // mark all squares around king as controlled
        uint64_t enemyKing = getOccupied(!pov) & kings;
        for (int left : {-1, 0, 1}) {
            for (int up : {-1, 0, 1}) {
                if (left == 0 && up == 0) continue;
                controlled |= applyOffset(left, up, enemyKing & canMoveToMask(left, up));
            }
        }
    }

    void Bitboard::evalKnightAttack() const {
        int leftArray[] = {-2, -2, -1, -1, 1, 1, 2, 2};
        int upArray[] = {-1, 1, -2, 2, -2, 2, -1, 1};

        uint64_t ownKing = getOccupied(pov) & kings;
        uint64_t enemyKnights = getOccupied(!pov) & knights;
        for (int i = 0; i < 8; i++) {
            int left = leftArray[i];
            int up = upArray[i];

            uint64_t enemyKnightControlled = applyOffset(left, up, enemyKnights & canMoveToMask(left, up));
            controlled |= enemyKnightControlled;

            checks |= applyOffset(-left, -up, enemyKnightControlled & ownKing);


        }

    }

    void Bitboard::parseFEN(std::string_view fen) {
        auto board_fen_end = fen.find(' ');
        auto color_fen_end = fen.find(' ', board_fen_end + 1);
        auto castling_fen_end = fen.find(' ', color_fen_end + 1);
        auto en_passant_fen_end = fen.find(' ', castling_fen_end + 1);
        //auto half_move_fen_end = fen.find(' ', en_passant_fen_end+1);


        std::string_view board_fen = fen.substr(0, board_fen_end);
        std::string_view color_fen = fen.substr(board_fen_end + 1, 1);
        std::string_view castling_fen = fen.substr(color_fen_end + 1, castling_fen_end - color_fen_end - 1);
        std::string_view en_passant_fen = fen.substr(castling_fen_end + 1, en_passant_fen_end - castling_fen_end - 1);

        parseBoardFEN(board_fen);
        parsePovFEN(color_fen);
        parseCastlingFEN(castling_fen);
        parseEnPassantFEN(en_passant_fen);

    }

    void Bitboard::parseBoardFEN(std::string_view boardFen) {
        kings = queens = rooks = knights = bishops = pawns = occupiedWhite = occupiedBlack = 0;
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
                occupiedWhite |= current;
            } else {
                occupiedBlack |= current;
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

    void Bitboard::parsePovFEN(std::string_view povFen) {
        assert(povFen == "w" || povFen == "b");
        pov = povFen == "w";
    }

    void Bitboard::parseCastlingFEN(std::string_view castlingFen) {
        castlingRights.reset();
        std::string alphaRights = "KQkq";
        for (unsigned i = 0; i < alphaRights.length(); i++) {
            castlingRights[i] = (castlingFen.find(alphaRights[i]) != std::string_view::npos);
        }
    }

    void Bitboard::parseEnPassantFEN(std::string_view enPassantFen) {
        if (enPassantFen == "-") {
            enPassantFile.reset();
        } else {
            assert(enPassantFen[0] <= 'h');
            assert(enPassantFen[0] >= 'a');
            enPassantFile = 7 - (enPassantFen[0] - 'a');
        }
    }

    std::string Bitboard::to_fen() const { throw std::logic_error("Not implemented!"); }

    std::string Bitboard::to_string() const {
        std::string retVal(8 * 9, '.');
        uint64_t currentidx = 1;
        currentidx <<= 63u;
        for (uint i = 0; i < retVal.size(); i++) {
            if (i % 9 == 8) {
                retVal[i] = '\n';
                continue;
            }
            if (((occupiedBlack | occupiedWhite) & currentidx) != 0) {
                bool white = (occupiedWhite & currentidx) != 0;
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


    void Bitboard::startpos() {
        kings = (0x08ull << 7u * 8u) | (0x08ull);
        queens = kings << 1u;
        bishops = kings >> 1u | queens << 1u;
        knights = kings >> 2u | queens << 2u;
        rooks = kings >> 3u | queens << 3u;
        pawns = (0xffull << 6u * 8u) | (0xff00ull);
        occupiedWhite = 0xffffull;
        occupiedBlack = occupiedWhite << 6u * 8u;

        pov = true;
        castlingRights.set();
        enPassantFile.reset();
    }

    void Bitboard::applyMoveSelf(const Move &move) {
        uint64_t fromMask = 1ull<<move.fromSquare;
        uint64_t toMask = 1ull<<move.toSquare;

        bool isPawn = pawns & fromMask;
        bool isKing = kings & fromMask;
        bool toBackRank = move.toSquare < 8 || move.toSquare > 55;

        // promotion
        if (isPawn && toBackRank) {
            preApplyPromotion(fromMask, move);
        }
        // castling
        if (isKing && move.fileDistance() == 2) {
            preApplyCastling(move);
        }
        // en passant capture (pawn capturing on a unoccupied square)
        if (isPawn && move.fileDistance() == 1 && toMask & ~occupied()){
            preApplyEnPassantCapture(move.toSquare);
        }
        // toggle en passant
        preApplyToggleEnPassant(move);
        // disable castle (king)
        if (isKing) {
            preApplyRemoveCastlingKingMove();
        }
        // disable castle (rook)
        preApplyRemoveCastlingRook(move);
        // capture or pawn
        if (isPawn || toMask&occupied()) {
            halfMoveCounter = 0;
        }
        else {
            halfMoveCounter += 1;
        }
        // increment move counter
        moveCounter += pov ? 0 : 1;
        // play actual move
        movePiece(move.fromSquare, move.toSquare);
        // flip pov
        pov = !pov;
        // check if enPassant can be captured legally
        //evalEnPassantLegality();
    }

    void Bitboard::preApplyPromotion(uint64_t fromMask, const Move &move) {
        // replace piece with promoted piece
        pawns &= ~fromMask;
        assert(move.promotion.has_value());

        switch(move.promotion.value()) {
            case 'q':
                queens |= fromMask;
                break;
            case 'r':
                rooks |= fromMask;
                break;
            case 'b':
                bishops |= fromMask;
                break;
            case 'n':
                knights |= fromMask;
                break;
            default:
                assert(false);
        }
    }

    void Bitboard::preApplyCastling(const Move &move) {
        // move rook to correct position
        unsigned rookOrigin = 0;
        unsigned rookTarget = 2;
        if (move.toSquare == 5) {
            rookOrigin = 7;
            rookTarget = 4;
        }
        else if (move.toSquare == 57) {
            rookOrigin = 56;
            rookTarget = 58;
        }
        else if (move.toSquare == 61) {
            rookOrigin = 63;
            rookTarget = 60;
        }
        else {
            assert(move.toSquare == 1);
        }
        rooks &= ~(1ull << rookOrigin);
        rooks |= 1ull << rookTarget;

    }

    void Bitboard::preApplyEnPassantCapture(unsigned int toSquare) {
        unsigned capturedPawnSquare = toSquare + ( pov ? -8 : 8);
        // remove pawn
        uint64_t capturedPawnMask = 1ull << capturedPawnSquare;
        pawns &= ~capturedPawnMask;
        occupiedBlack &= ~capturedPawnMask;
        occupiedWhite &= ~capturedPawnMask;
    }

    void Bitboard::preApplyRemoveCastlingKingMove() {
        castlingRights[pov ? 0 : 2] = false;
        castlingRights[pov ? 1 : 3] = false;
    }

    void Bitboard::preApplyRemoveCastlingRook(const Move &move) {
        unsigned rookSquares[] = {0,7,56,63};
        for (int i = 0; i < 4; i++) {
            if (move.toSquare == rookSquares[i] || move.fromSquare == rookSquares[i]) {
                castlingRights[i] = false;
            }
        }
    }

    void Bitboard::preApplyToggleEnPassant(const Move &move) {
        uint64_t fromMask = 1ull << move.fromSquare;
        bool isPawn = fromMask & pawns;
        enPassantFile = std::nullopt;
        if (!isPawn || move.rankDistance() != 2) {
            return;
        }
        // set en passant
        // legality of en passant will be evaluated at a later time
        uint64_t enPassantRankMask = 0xffull << ((pov ? 3 : 4)*8);
        uint64_t pushedPawnMask = 1ull << move.toSquare;
        uint64_t captureSquaresMask = ((pushedPawnMask << 1) | (pushedPawnMask >> 1)) & enPassantRankMask;
        bool enemyPawnOnRelevantSquare = captureSquaresMask & pawns & getOccupied(!pov);
        if (enemyPawnOnRelevantSquare) {
            enPassantFile = move.toSquare % 8;
        }

    }

    void Bitboard::movePiece(unsigned int fromSquare, unsigned int toSquare) {
        uint64_t fromMask = 1ull << fromSquare;
        uint64_t toMask = 1ull << toSquare;

        assert(fromMask & getOccupied(pov));

        uint64_t otherMask = ~(fromMask | toMask);

        kings = (kings & otherMask) | ((kings & fromMask) != 0) * toMask;
        queens = (queens & otherMask) | ((queens & fromMask) != 0) * toMask;
        bishops = (bishops & otherMask) | ((bishops & fromMask) != 0) * toMask;
        knights = (knights & otherMask) | ((knights & fromMask) != 0) * toMask;
        rooks = (rooks & otherMask) | ((rooks & fromMask) != 0) * toMask;
        pawns = (pawns & otherMask) | ((pawns & fromMask) != 0) * toMask;
        occupiedWhite = (occupiedWhite & otherMask) | ((occupiedWhite & fromMask) != 0) * toMask;
        occupiedBlack = (occupiedBlack & otherMask) | ((occupiedBlack & fromMask) != 0) * toMask;
    }

    bool Bitboard::operator==(const Bitboard &other) const {
        // does not compare en passant, pov, castling, ...
        throw std::logic_error("Not implemented!");
        return
                kings == other.kings &&
                queens == other.queens &&
                rooks == other.rooks &&
                knights == other.knights &&
                bishops == other.bishops &&
                pawns == other.pawns &&
                occupiedWhite == other.occupiedWhite &&
                occupiedBlack == other.occupiedBlack;
    }


    /**
     * Calculates a mask of all squares that can move to the given direction without wrapping the border
     * @param left Amount to move towards the A file. If negative move toward H file.
     * @param up Amount to move toward the 8th rank. If negative move towards 1st rank.
     * @return mask
     */
    constexpr uint64_t Bitboard::canMoveToMask(int left, int up) {
        assertOffset(left, up);
        // 11111111
        uint8_t rowMask = ~static_cast<uint8_t>(0);
        if (left > 0) {
            // 00111111
            rowMask >>= left;
        }
        if (left < 0) {
            // 11111000
            rowMask <<= (-left);
        }
        uint64_t mask;
        std::memset(&mask, rowMask, 8);
        if (up > 0) {
            // erase top rows
            mask >>= 8u * up;
        }
        if (up < 0) {
            // erase bottom rows
            mask <<= 8u * (-up);
        }
        return mask;
    }

    /**
     * shifts a single bitmap of a board by the given amount
     * @param left Amount to move towards the A file. If negative move toward H file.
     * @param up Amount to move toward the 8th rank. If negative move towards 1st rank.
     * @param board bitmap to be shifted
     * @return bitmap of the shifted board
     */
    constexpr uint64_t Bitboard::applyOffset(int left, int up, uint64_t board) {
        assertOffset(left, up);

        int offset = left + 8 * up;
        return offset > 0 ? board << offset : board >> (-offset);

    }

    Bitboard Bitboard::applyMoveCopy(const Move &move) const {
        Bitboard nextBoard = *this;
        nextBoard.applyMoveSelf(move);
        return nextBoard;
    }

    uint64_t &Bitboard::relevantPinMap(int dx, int dy) const {
        assert (dx != 0 || dy != 0);

        if (dx == 0) return pinnedVertical;
        if (dy == 0) return pinnedHorizontal;
        if (dx == dy) return pinnedDiagonal;

        assert(dx == -dy);
        return pinnedAntidiagonal;
    }

} // namespace chess