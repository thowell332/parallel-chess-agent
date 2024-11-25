/**
 * @file AlphaBetaTest.cpp
 * @brief Implements unit tests and timing tests for minimax algorithms with alpha-beta pruning.
 */

#include "../AlphaBeta.hpp"

#include <chess.hpp>
#include <memory>

int main(int argc, char* argv[])
{
    auto root = std::make_unique<GameNode>();
    auto bestMove = alphaBetaSeq<chess::Color::WHITE>(*root, 5);
    std::cout << "Best move: " << bestMove << " with score = " << bestMove.score() << std::endl;
    // TODO: Implement unit tests and timing tests
}
