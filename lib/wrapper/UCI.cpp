
#include <iostream>
#include "UCI.hpp"

namespace chess {
    void UCI::start() {
        std::string lineStr;
        while (!quitting && getline(instream,lineStr)) {
            line = std::istringstream(lineStr);
            std::cerr << lineStr << std::endl;
            std::string cmd;
            line >> cmd;
            if (cmd == "uci") uci();
            else if(cmd == "debug") debug();
            else if(cmd == "position") position();
            else if(cmd == "isready") isready();
            else if(cmd == "go") go();
            else if(cmd == "quit") quit();
            else{
                _unknown();
            }
        }
    }

    void chess::UCI::uci() {
        outstream << "id name MChessTS" << std::endl;
        outstream << "id author waegemans" << std::endl;
        outstream << "uciok" << std::endl;
    }

    void UCI::debug() {
        _notImplemented();
    }

    void UCI::isready() {
        outstream << "readyok" << std::endl;
    }

    void UCI::setoption() {
        _notImplemented();
    }

    void UCI::registerUCI() {
        _notImplemented();
    }

    void UCI::ucinewgame() {
        _notImplemented();
    }

    void UCI::position() {
        std::string mode;
        line >> mode;
        if (mode == "fen"){
            std::string fen,pov,castling,enpassant,halfmove,fullmove;
            line >> fen >> pov >> castling >> enpassant >> halfmove >> fullmove;
            state.parseFen(fen+" "+pov+" "+castling+" "+enpassant+" "+halfmove+" "+fullmove);
        }
        else if (mode == "startpos") {
            state.reset();
        }
        std::string moveUCI;
        if (line >> moveUCI && moveUCI == "moves"){
            while (line.rdbuf()->in_avail() && line >> moveUCI) {
                state.applyMoveSelf(Move(moveUCI));
            }
        }

    }

    void UCI::go() {
        auto nextMove = search.findNextMove(state, {0,0,5000,5000});
        outstream << "bestmove " << nextMove.toUCI() << std::endl;
    }

    void UCI::quit() {
        quitting = true;
    }

    void UCI::stop() {
        _notImplemented();
    }

    void UCI::ponderhit() {
        _notImplemented();
    }

    void UCI::_notImplemented() {
        std::cerr << "info not implemented" << std::endl;
    }

    void UCI::_unknown() {
        std::cerr << "info unknown command" << std::endl;
    }
}