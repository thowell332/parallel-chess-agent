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

// Tag dispatching for algorithm execution policy
struct SequentialTag {};
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
chess::Move alphaBeta(
    [[maybe_unused]] const SequentialTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha = eval_constants::MIN_SCORE,
    std::int16_t beta = eval_constants::MAX_SCORE,
    bool isMaximizingPlayer = true
);

/**
 * @brief Parallel shared-memory implementation of the minimax algorithm
 * with alpha-beta pruning.
 */
chess::Move alphaBeta(
    [[maybe_unused]] const SharedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha = eval_constants::MIN_SCORE,
    std::int16_t beta = eval_constants::MAX_SCORE,
    bool isMaximizingPlayer = true
);

#endif // ALPHA_BETA_HPP
