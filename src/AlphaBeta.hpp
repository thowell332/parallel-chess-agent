/**
 * @file AlphaBeta.hpp
 */

#ifndef ALPHA_BETA_HPP
#define ALPHA_BETA_HPP

#include "GameState.hpp"

#include <chess.hpp>
#include <algorithm>
#include <cstdint>
#include <exception>

/**
 * @brief Sequential minimax algorithm with alpha-beta pruning.
 * 
 * @param GameNode Current node in the game tree.
 * @param maxDepth Maximum depth to explore.
 * @param depth Current depth in game tree.
 * @param alpha Best value that the maximizer can guarantee at this level or above.
 * @param beta Best value that the minimizer can guarantee at this level or above.
 * 
 * @return Best move that the maximizing player can make with associated score.
 */
template <chess::Color::underlying color>
chess::Move alphaBetaSeq(
    const GameNode& gameNode,
    std::uint8_t maxDepth,
    std::uint8_t depth = 0,
    std::int16_t alpha = MIN_SCORE,
    std::int16_t beta = MAX_SCORE
) {
    // Verify input parameters
    if (maxDepth == 0) {
        throw std::invalid_argument("Maximum depth must be nonzero.");
    }
    if (alpha < MIN_SCORE || alpha > MAX_SCORE) {
        throw std::out_of_range("Alpha out of bounds.");
    }
    if (beta < MIN_SCORE || beta > MAX_SCORE) {
        throw std::out_of_range("Beta out of bounds.");
    }

    // Return if depth reaches maximum depth or there are no legal moves
    if (depth >= maxDepth || gameNode.children().empty()) {
        auto move = gameNode.lastMove();
        auto score = gameNode.scoreLastMove<color>();
        move.setScore(score);
        return move;
    }

    chess::Move bestMove;
    // Maximizing player seeks the highest score
    if (gameNode.isMyTurn<color>()) {
        bestMove.setScore(MIN_SCORE);
        for (const auto& child : gameNode.children()) {
            auto move = alphaBetaSeq<color>(*child, maxDepth, depth + 1, alpha, beta);
            auto score = move.score();
            if (score > bestMove.score()) {
                bestMove = child->lastMove();
                bestMove.setScore(score);
            }
            alpha = std::max(alpha, bestMove.score());
            if (beta <= alpha) {
                break;
            }
        }
    }
    // Minimizing player seeks the lowest score
    else {
        bestMove.setScore(MAX_SCORE);
        for (const auto& child : gameNode.children()) {
            auto move = alphaBetaSeq<color>(*child, maxDepth, depth + 1, alpha, beta);
            auto score = move.score();
            if (score < bestMove.score()) {
                bestMove = child->lastMove();
                bestMove.setScore(score);
            }
            beta = std::min(beta, bestMove.score());
            if (beta <= alpha) {
                break;
            }
        }
    }
    return bestMove;
}

#endif // ALPHA_BETA_HPP
