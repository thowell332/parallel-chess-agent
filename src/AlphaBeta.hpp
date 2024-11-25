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

namespace Sequential {
    /**
     * @brief Sequential minimax algorithm with alpha-beta pruning.
     * 
     * @param GameNode Current node in the game tree.
     * @param maxDepth Maximum depth to explore.
     * @param depth Current depth in game tree.
     * @param alpha Best value that the maximizer can guarantee at this level or above.
     * @param beta Best value that the minimizer can guarantee at this level or above.
     * @param isMaximizingPlayer Indicates whether the active player is the maximizing player.
     * 
     * @return Best move that the maximizing player can make with associated score.
     */
    chess::Move alphaBeta(
        const GameNode& gameNode,
        std::uint8_t maxDepth,
        std::uint8_t depth = 0,
        std::int16_t alpha = EvalConstants::MIN_SCORE,
        std::int16_t beta = EvalConstants::MAX_SCORE,
        bool isMaximizingPlayer = true
    );
} // namespace Sequential

#endif // ALPHA_BETA_HPP
