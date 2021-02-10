
#include <string>
#include <iostream>
#include "eval/PiecePositionEvaluator.hpp"
#include "wrapper/UCI.hpp"
#include "search/AlphaBetaSearch.hpp"

int main() {
    chess::PiecePositionEvaluator piecePositionEvaluator;
    chess::AlphaBetaSearch search(piecePositionEvaluator);
    chess::UCI uci(std::cout,std::cin,search);
    uci.start();
}

