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
AlphaBetaResult alphaBeta(
    const SequentialTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha = eval_constants::MIN_SCORE,
    std::int16_t beta = eval_constants::MAX_SCORE,
    bool isMaximizingPlayer = true
);

// Naive shared memory parallel implementation
AlphaBetaResult alphaBeta(
    const NaiveSharedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha = eval_constants::MIN_SCORE,
    std::int16_t beta = eval_constants::MAX_SCORE,
    bool isMaximizingPlayer = true
);

// Shared memory parallel implementation
AlphaBetaResult alphaBeta(
    const SharedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha = eval_constants::MIN_SCORE,
    std::int16_t beta = eval_constants::MAX_SCORE,
    bool isMaximizingPlayer = true
);

// Shared memory with occasional sync'ing implementation
AlphaBetaResult alphaBeta(
    const GameNode& gameNode,
    std::uint8_t depth,
    const std::uint8_t numSyncInterations, // number of iterations before sync'ing
    std::uint8_t curSyncIteration = 0,
    std::int16_t *globalAlpha = new std::int16_t, // globals must be instantiated outside of function call
    std::int16_t *globalBeta = new std::int16_t,
    std::int16_t alpha = eval_constants::MIN_SCORE,
    std::int16_t beta = eval_constants::MAX_SCORE,
    bool isMaximizingPlayer = true
);

// Distributed memory parallel implementation
AlphaBetaResult alphaBeta(
    const DistributedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
);

#endif // ALPHA_BETA_HPP
