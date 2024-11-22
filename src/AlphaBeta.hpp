/**
 * @file AlphaBeta.hpp
 */

#ifndef ALPHA_BETA_HPP
#define ALPHA_BETA_HPP

#include "GameState.hpp"

#include <chess.hpp>
#include <algorithm>
#include <cstdint>

/**
 * @brief Sequential minimax algorithm with alpha-beta pruning.
 * 
 * @param gameState Current game state.
 * @param maxDepth Maximum depth to explore.
 * @param depth Current depth in game tree.
 * @param alpha Best value that the maximizer can guarantee at this level or above.
 * @param beta Best value that the minimizer can guarantee at this level or above.
 * 
 * @return Best move that the maximizing player can make with associated score.
 */
template <chess::Color::underlying color>
chess::Move alphaBetaSeq(
    const GameState & gameState,
    int maxDepth,
    int depth = 0,
    std::int16_t alpha = MIN_SCORE,
    std::int16_t beta = MAX_SCORE
) {
    // Return if depth exceeds maximum depth
    if (depth > maxDepth) {
        return gameState.lastMoveScored();
    }

    // Generate all legal moves from this board state
    chess::Movelist movelist;
    chess::movegen::legalmoves(movelist, gameState.board());

    // Return if there are no legal moves
    if (movelist.empty()) {
        return gameState.lastMoveScored();
    }

    chess::Move bestMove;
    if (chess::Color(color) == gameState.board().sideToMove()) {
        bestMove.setScore(MIN_SCORE);
        std::int16_t newAlpha = alpha;
        for (auto itr = movelist.begin(); itr != movelist.end(); ++itr) {
            auto newGameState = gameState;
            newGameState.makeMove(*itr);
            auto bestNextMove = alphaBetaSeq<color>(newGameState, depth + 1, maxDepth, newAlpha, beta);
            if (bestNextMove.score() > bestMove.score()) {
                bestMove = *itr;
                bestMove.setScore(bestNextMove.score());
            }
            newAlpha = std::max(newAlpha, bestMove.score());
            if (beta <= newAlpha) {
                break;
            }
        }
    } else {
        bestMove.setScore(MAX_SCORE);
        std::int16_t newBeta = beta;
        for (auto itr = movelist.begin(); itr != movelist.end(); ++itr) {
            auto newGameState = gameState;
            newGameState.makeMove(*itr);
            auto bestNextMove = alphaBetaSeq<color>(newGameState, depth + 1, maxDepth, alpha, newBeta);
            if (bestNextMove.score() < bestMove.score()) {
                bestMove = *itr;
                bestMove.setScore(bestNextMove.score());
            }
            newBeta = std::min(newBeta, bestMove.score());
            if (newBeta <= alpha) {
                break;
            }
        }
    }
    return bestMove;
}

#endif // ALPHA_BETA_HPP
