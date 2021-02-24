#include "State.hpp"

#include <cassert>
#include <bit>

namespace chess {
    namespace {
        uint64_t getMovablesQueenLike(int dx, int dy, int i, bool pov, uint64_t &lastFree, const Bitboard &bitboard) {
            auto free = bitboard.freeSquares(dy, dx, pov);
            auto take = bitboard.takeSquares(dy, dx, pov);
            uint64_t movable = lastFree & (free | take);
            if (i == 1) {
                movable |= bitboard.getKings() & (free | take);
            }
            lastFree &= lastFree & free;

            return movable;
        }

        void fillMove(uint64_t movable, int dx, int dy, std::unordered_set<Move, HashMove> &result,
                      uint64_t promotable = 0) {
            for (unsigned bitPos = 0; bitPos < 64; bitPos++) {
                uint64_t bitMask = (1ull << bitPos);
                if ((bitMask & movable) != 0) {
                    auto toPos = bitPos - dx + dy * 8;
                    assert(toPos < 64);
                    if ((bitMask & promotable) != 0) {
                        for (char c : {'q', 'r', 'n', 'b'}) {
                            result.emplace(bitPos, toPos, c);
                        }
                    } else {
                        result.emplace(bitPos, toPos);
                    }
                }
            }
        }

        void queenLikeMoves(const Bitboard &bitboard, bool pov, std::unordered_set<Move, HashMove> &result) {
            auto orthogonalPieces = bitboard.getRooks() | bitboard.getQueens();
            auto diagonalPieces = bitboard.getBishops() | bitboard.getQueens();

            for (int xscale = -1; xscale < 2; ++xscale) {
                for (int yscale = -1; yscale < 2; ++yscale) {
                    if (yscale == 0 && xscale == 0)
                        continue;

                    bool orthogonal = yscale == 0 || xscale == 0;
                    auto majorPieces = orthogonal ? orthogonalPieces : diagonalPieces;
                    uint64_t lastFree = majorPieces;

                    for (int i = 1; i < 8; i++) {
                        auto dx = xscale * i;
                        auto dy = yscale * i;
                        uint64_t movable = getMovablesQueenLike(dx, dy, i, pov, lastFree, bitboard);
                        fillMove(movable, dx, dy, result);
                    }
                }
            }
        }

        void pawnMoves(const Bitboard &bitboard, bool pov, std::unordered_set<Move, HashMove> &result) {
            int ydir = pov ? 1 : -1;
            uint64_t promotableMask = pov ? 0xffull << 6u * 8u : 0xff00ull;
            uint64_t startPosMask = !pov ? 0xffull << 6u * 8u : 0xff00ull;
            uint64_t promotablePieces = bitboard.getPawns() & promotableMask;

            uint64_t movable1 = bitboard.getPawns() & bitboard.freeSquares(ydir, 0, pov);
            uint64_t movable1right = bitboard.getPawns() & bitboard.takeSquares(ydir, 1, pov);
            uint64_t movable1left = bitboard.getPawns() & bitboard.takeSquares(ydir, -1, pov);

            uint64_t movable2 = movable1 & startPosMask & bitboard.freeSquares(ydir * 2, 0, pov);

            fillMove(movable1, 0, ydir, result, promotablePieces);
            fillMove(movable1right, 1, ydir, result, promotablePieces);
            fillMove(movable1left, -1, ydir, result, promotablePieces);
            fillMove(movable2, 0, 2 * ydir, result);
        }

        void enPassantMoves(const Bitboard& bitboard, bool pov, std::optional<unsigned> enPassantFile, std::unordered_set<Move, HashMove>& result) {
            int ydir = pov ? 1 : -1;
            if (!enPassantFile.has_value()) return;
            uint64_t enPassantRight = 0b10 << (enPassantFile.value());
            enPassantRight <<= pov ? 4u*8u : 3u*8u;
            uint64_t enPassantLeft = enPassantRight >> 2;

            uint64_t relevantRowMask = 0xffull << (pov ?  4u*8u : 3u*8u);
            uint64_t pawnsOnRelevantRow = bitboard.getPawns() & bitboard.getOccupiedPov(pov) & relevantRowMask;

            uint64_t takeRight = enPassantRight & pawnsOnRelevantRow;
            uint64_t takeLeft = enPassantLeft & pawnsOnRelevantRow;

            fillMove(takeRight,1,ydir,result);
            fillMove(takeLeft,-1,ydir,result);

        }

        void knightMoves(const Bitboard &bitboard, bool pov, std::unordered_set<Move, HashMove> &result) {
            int dxArray[] = {-2, -2, -1, -1, 1, 1, 2, 2};
            int dyArray[] = {-1, 1, -2, 2, -2, 2, -1, 1};

            for (unsigned idx = 0; idx < 8; idx++) {
                int dx = dxArray[idx];
                int dy = dyArray[idx];
                uint64_t movable =
                        (bitboard.freeSquares(dy, dx, pov) | bitboard.takeSquares(dy, dx, pov)) & bitboard.getKnights();
                fillMove(movable, dx, dy, result);
            }
        }

        void castlingMoves(const Bitboard& bitboard, bool pov, std::bitset<4> castlingRights, std::unordered_set<Move, HashMove>& result) {
            bool has_kingside = castlingRights[!pov*2+1];
            bool has_queenside = castlingRights[!pov*2];
            if (!has_kingside && !has_queenside) return;

            uint64_t occupied = bitboard.occupied();
            unsigned offset = pov ? 0 : 8*7;
            occupied >>= offset;
            occupied &= 0xff;

            bool can_kingside = has_kingside & ((occupied&0b110) == 0);
            bool can_queenside = has_queenside & ((occupied&0b1110000) == 0);

            if (can_kingside) {
                result.emplace(3 + offset,1 + offset);
            }
            if(can_queenside) {
                result.emplace(3+offset,5+offset);
            }
        }

    }

    void State::reset() {
        bitboard.startpos();
        pov = true;
        castlingRights.set();
        enPassantFile.reset();

        isCheckCache.reset();
        isGameOverCache.reset();
    }

    void State::parseFen(std::string_view fen) {
        reset();
        auto board_fen_end = fen.find(' ');
        auto color_fen_end = fen.find(' ', board_fen_end+1);
        auto castling_fen_end = fen.find(' ', color_fen_end+1);
        auto en_passant_fen_end = fen.find(' ', castling_fen_end+1);
        //auto half_move_fen_end = fen.find(' ', en_passant_fen_end+1);


        std::string_view board_fen = fen.substr(0, board_fen_end);
        std::string_view color_fen = fen.substr(board_fen_end + 1, 1);
        std::string_view castling_fen = fen.substr(color_fen_end + 1, castling_fen_end-color_fen_end-1);
        std::string_view en_passant_fen = fen.substr(castling_fen_end+1, en_passant_fen_end - castling_fen_end-1);


        bitboard.parse_fen(board_fen);
        assert (color_fen == "w" or color_fen == "b");
        pov = (color_fen == "w");
        parseCastlingFen(castling_fen);
        parseEnPassantFen(en_passant_fen);

    }

    void State::parseCastlingFen(std::string_view castling_fen) {
        castlingRights.reset();
        std::string alphaRights = "KQkq";
        for (unsigned i = 0; i < alphaRights.length(); i++) {
            castlingRights[i] = (castling_fen.find(alphaRights[i]) != std::string_view::npos);
        }
    }

    void State::parseEnPassantFen(std::string_view en_passant_fen) {
        if (en_passant_fen == "-") {
            enPassantFile.reset();
        }
        else {
            enPassantFile = 7-(en_passant_fen[0]-'a');
        }
    }

    std::unordered_set<Move, HashMove> State::pseudoLegalMoves(bool flipPov) const {
        std::unordered_set<Move, HashMove> result;
        queenLikeMoves(bitboard, pov^flipPov, result);
        pawnMoves(bitboard, pov^flipPov, result);
        knightMoves(bitboard, pov^flipPov, result);
        castlingMoves(bitboard, pov^flipPov, castlingRights, result);
        enPassantMoves(bitboard, pov^flipPov, enPassantFile, result);
        return result;
    }

    std::unordered_set<Move, HashMove> State::legalMoves() const {
        auto pseudoLegal = pseudoLegalMoves();
        std::unordered_set<Move, HashMove> legal;
        for (const auto& move: pseudoLegal) {
            //castling move
            if (((1ull<<move.fromSquare)&bitboard.getKings()) != 0 && move.fileDistance() == 2) {
                // cannot castle while checked
                if (isCheck()) continue;

                // cannot castle over controlled square
                Move oneStepKingMove(move.fromSquare, move.fromSquare<move.toSquare ? move.toSquare-1: move.toSquare+1);
                auto oneStepKingState = applyMove(oneStepKingMove);
                if (!oneStepKingState.isLegal()) continue;

            }
            auto nextState = applyMove(move);
            if (nextState.isLegal()) legal.insert(move);
        }
        return legal;
    }

    bool State::isLegal(bool flipPov) const {
        uint64_t ownKings = bitboard.getKings() & bitboard.getOccupiedPov(pov^flipPov);
        uint64_t enemyKings = bitboard.getKings() & bitboard.getOccupiedPov(!pov^flipPov);
        if (std::popcount(ownKings) != 1 || std::popcount(enemyKings) != 1) return false;

        unsigned enemyKingPos = __builtin_ctzll(enemyKings);

        // check if enemy king can be captured
        for (const auto& move : pseudoLegalMoves(flipPov) ){
            if (move.toSquare == enemyKingPos)
                return false;
        }
        return true;
    }

    State State::applyMove(Move move) const {
        State newState = *this;
        newState.applyMoveSelf(move);
        return newState;
    }

    void State::applyMoveSelf(Move move) {
        isGameOverCache.reset();
        isCheckCache.reset();
        uint64_t fromMask = 1ull<<move.fromSquare;
        uint64_t toMask = 1ull<<move.toSquare;

        assert(fromMask&bitboard.getOccupiedPov(pov));

        // disable castling because of rook
        unsigned corners[] = {0,7,56,63};
        for (unsigned i = 0; i < 4; i++) {
            if (move.fromSquare == corners[i] || move.toSquare == corners[i]) {
                castlingRights[i] = false;
            }
        }

        // king moved disable castling
        if (bitboard.getKings()&fromMask) {
            castlingRights[2*!pov] = false;
            castlingRights[2*!pov+1] = false;
        }


        if (bitboard.getKings()&fromMask && move.fileDistance() > 1) {
            //castling move -> move rook first
            if (move.toSquare == 1) bitboard.applyMove(0, 2);
            if (move.toSquare == 5) bitboard.applyMove(7, 4);
            if (move.toSquare == 57) bitboard.applyMove(56, 58);
            if (move.toSquare == 61) bitboard.applyMove(63, 60);
        }
        // promotion -> replace with target piece before moving
        uint64_t promotable = 0xffull << (pov ? 6u*8u : 8u);
        if (bitboard.getPawns()&fromMask&promotable) {
            assert(move.promotion.has_value());
            bitboard.pawns &= ~fromMask;
            switch (move.promotion.value()) {
                case 'q':
                    bitboard.queens |= fromMask;
                    break;
                case 'r':
                    bitboard.rooks |= fromMask;
                    break;
                case 'b':
                    bitboard.bishops |= fromMask;
                    break;
                case 'n':
                    bitboard.knights |= fromMask;
                    break;
            }
        }
        // en passant move
        if (bitboard.getPawns()&fromMask && move.fileDistance() == 1 && (bitboard.getOccupiedPov(!pov)&toMask) == 0) {
            uint64_t eraseMask = pov ? toMask >> 8u : toMask << 8u;
            bitboard.pawns &= ~eraseMask;
            if (pov) bitboard.occupied_black &= ~eraseMask;
            else bitboard.occupied_white &= ~eraseMask;
        }
        if (bitboard.getPawns()&fromMask && move.rowDistance() > 1) {
            // activate en passant for double pawn move
            enPassantFile = move.toSquare%8;
        }
        else {
            enPassantFile.reset();
        }

        bitboard.applyMove(move.fromSquare,move.toSquare);
        pov = !pov;
    }

    bool State::isCheck() const{
        if (isCheckCache.has_value()) return isCheckCache.value();

        bool retVal = !isLegal(true);
        isCheckCache = retVal;
        return retVal;
    }

    bool State::isGameOver() const{
        if (isGameOverCache.has_value()) return isGameOverCache.value();
        bool retVal;
        if (isInsufficient()) retVal = true;
        else retVal = legalMoves().empty();
        isGameOverCache = retVal;
        return retVal;
    }

    bool State::isInsufficient() const {
        unsigned pieceCount = std::popcount(bitboard.occupied_black|bitboard.occupied_white);
        // king vs king
        if (pieceCount == 2) return true;
        // king vs king and (knight | bishop)
        if (pieceCount == 3 && (bitboard.knights | bitboard.bishops) != 0) return true;
        // king and bishop vs king and bishop (same square)
        if (pieceCount == 4 && std::popcount(bitboard.bishops) == 2 &&  std::popcount(bitboard.bishops&0xAA55AA55AA55AA55ull) != 1) return true;
        return false;
    }

    bool State::canClaimDraw(const std::vector<State> &previousStates) const {
        if (previousStates.empty()) {
            return false;
        }
        return false;
    }

    bool State::operator==(const State &other) const {
        return
        bitboard == other.bitboard &&
        pov == other.pov &&
        castlingRights == other.castlingRights &&
        enPassantFile == other.enPassantFile;
    }

}
