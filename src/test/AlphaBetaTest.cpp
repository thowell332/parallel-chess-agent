/**
 * @file AlphaBetaTest.cpp
 * @brief Implements unit tests and timing tests for minimax algorithms with alpha-beta pruning.
 */

#include "../AlphaBeta.hpp"

#include <chess.hpp>

int main(int argc, char* argv[])
{
    GameState gameState;
    auto bestMove = alphaBetaSeq<chess::Color::WHITE>(gameState, 4);
    std::cout << "bestMove: " << bestMove << std::endl;
    // TODO: Implement unit tests and timing tests
}
