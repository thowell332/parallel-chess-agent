/**
 * @file AlphaBeta.cpp
 */

#include "AlphaBeta.hpp"

chess::Move Sequential::alphaBeta(
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
) {
    // Verify input parameters
    if (alpha < EvalConstants::MIN_SCORE || alpha > EvalConstants::MAX_SCORE) {
        throw std::out_of_range("Alpha out of bounds.");
    }
    if (beta < EvalConstants::MIN_SCORE || beta > EvalConstants::MAX_SCORE) {
        throw std::out_of_range("Beta out of bounds.");
    }

    // Return if the maximum depth has been explored or there are no legal moves remaining
    if (depth == 0 || gameNode.children().empty()) {
        auto move = gameNode.lastMove();
        auto activePlayerScore = gameNode.evaluateBoard();
        auto score = isMaximizingPlayer ? activePlayerScore : -activePlayerScore;
        move.setScore(score);
        return move;
    }

    chess::Move bestMove;
    if (isMaximizingPlayer) {
        bestMove.setScore(EvalConstants::MIN_SCORE - 1);
        for (const auto& child : gameNode.children()) {
            auto move = Sequential::alphaBeta(*child, depth - 1, alpha, beta, false);
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
    } else {
        bestMove.setScore(EvalConstants::MAX_SCORE + 1);
        for (const auto& child : gameNode.children()) {
            auto move = Sequential::alphaBeta(*child, depth - 1, alpha, beta, true);
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
