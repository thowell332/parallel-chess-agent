/**
 * @file AlphaBeta.hpp
 */

#ifndef ALPHA_BETA_HPP
#define ALPHA_BETA_HPP

#include "GameNode.hpp"
#include <chess.hpp>

#include <algorithm>
#include <cstdint>
#include <exception>

// Return type for alpha-beta pruning algorithms
struct AlphaBetaResult
{
    chess::Move bestMove;
    size_t nodesExplored;
};

// Tag dispatching for algorithm execution policy
struct SequentialTag {};
struct NaiveSharedMemoryTag {};
struct SharedMemoryTag {};
struct DistributedMemoryTag {};

/**
 * @brief Sequential minimax algorithm with alpha-beta pruning.
 * 
 * @param policy Execution policy (sequential or parallel).
 * @param gameNode Current node in the game tree.
 * @param depth Depth to explore in the game tree.
 * @param alpha Best value that the maximizer can guarantee at this level or above.
 * @param beta Best value that the minimizer can guarantee at this level or above.
 * @param isMaximizingPlayer Indicates whether the active player is the maximizing player.
 * 
 * @return Best move that the maximizing player can make with associated score.
 */
template<typename Tag>
AlphaBetaResult alphaBeta(
    const Tag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha = eval_constants::MIN_SCORE,
    std::int16_t beta = eval_constants::MAX_SCORE,
    bool isMaximizingPlayer = true
) {
    // Verify input parameters
    if (alpha < eval_constants::MIN_SCORE || alpha > eval_constants::MAX_SCORE) {
        throw std::out_of_range("Alpha out of bounds.");
    }
    if (beta < eval_constants::MIN_SCORE || beta > eval_constants::MAX_SCORE) {
        throw std::out_of_range("Beta out of bounds.");
    }

    // Return if the maximum depth has been explored or there are no legal moves remaining
    if (depth == 0 || gameNode.children().empty()) {
        auto move = gameNode.lastMove();
        auto activePlayerScore = gameNode.evaluateBoard();
        auto score = isMaximizingPlayer ? activePlayerScore : -activePlayerScore;
        move.setScore(score);
        return {move, 1};
    }

    // Recurse on children
    return alphaBetaRecurse(policy, gameNode, depth, alpha, beta, isMaximizingPlayer);
}

// Sequential implementation
AlphaBetaResult alphaBetaRecurse(
    const SequentialTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
);

// Naive shared memory parallel implementation
AlphaBetaResult alphaBetaRecurse(
    const NaiveSharedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
);

// Shared memory parallel implementation
AlphaBetaResult alphaBetaRecurse(
    const SharedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
);

// Distributed memory parallel implementation
AlphaBetaResult alphaBetaRecurse(
    const DistributedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
);

#endif // ALPHA_BETA_HPP
