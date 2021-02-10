#pragma once

#include <ostream>
#include <istream>
#include <string_view>
#include <sstream>
#include <search/Search.hpp>
#include "State.hpp"

namespace chess {
    class UCI {
    public:
        void start();
        UCI(std::ostream& outstream,
            std::istream& instream, Search& search) : outstream(outstream), instream(instream), search(search) {};
    private:
        std::ostream& outstream;
        std::istream& instream;
        bool quitting = false;
        State state;
        Search& search;
        std::istringstream line;


        void uci();

        void debug();

        void isready();

        void setoption();

        void registerUCI();

        void ucinewgame();

        void position();

        void go();

        void stop();

        void ponderhit();

        void quit();

        void _notImplemented();
        void _unknown();

    };

}
