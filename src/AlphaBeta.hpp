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
struct SharedCutoffsTag {};
struct LocalCutoffsTag {};
struct BlendedCutoffsTag {};

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

// Shared memory parallel implementation with shared cutoff values
AlphaBetaResult alphaBeta(
    const SharedCutoffsTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha = eval_constants::MIN_SCORE,
    std::int16_t beta = eval_constants::MAX_SCORE,
    bool isMaximizingPlayer = true
);

// Shared memory parallel implementation with local cutoff values
AlphaBetaResult alphaBeta(
    const LocalCutoffsTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha = eval_constants::MIN_SCORE,
    std::int16_t beta = eval_constants::MAX_SCORE,
    bool isMaximizingPlayer = true
);

// Shared memory implementation with perioduc cutoff synchronization
AlphaBetaResult alphaBeta(
    const BlendedCutoffsTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::uint8_t numSyncInterations = 1,
    std::int16_t alpha = eval_constants::MIN_SCORE,
    std::int16_t beta = eval_constants::MAX_SCORE,
    bool isMaximizingPlayer = true
);

#endif // ALPHA_BETA_HPP
