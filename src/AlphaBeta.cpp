/**
 * @file AlphaBeta.cpp
 */

#include "AlphaBeta.hpp"

#include <unordered_map>

chess::Move alphaBeta(
    const SequentialTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
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
        return move;
    }

    chess::Move bestMove;
    if (isMaximizingPlayer) {
        bestMove.setScore(eval_constants::MIN_SCORE - 1);
        for (const auto& child : gameNode.children()) {
            auto move = alphaBeta(SequentialTag{}, *child, depth - 1, alpha, beta, false);
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
        bestMove.setScore(eval_constants::MAX_SCORE + 1);
        for (const auto& child : gameNode.children()) {
            auto move = alphaBeta(SequentialTag{}, *child, depth - 1, alpha, beta, true);
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

chess::Move alphaBeta(
    const NaiveSharedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
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
        return move;
    }

    chess::Move bestMove;
    if (isMaximizingPlayer) {
        bestMove.setScore(eval_constants::MIN_SCORE - 1);

        #pragma omp parallel for
        for (const auto& child : gameNode.children()) {
            if (beta <= alpha) {
                continue;
            }
            auto move = alphaBeta(SequentialTag{}, *child, depth - 1, alpha, beta, false);
            auto score = move.score();
            if (score > bestMove.score()) {
                bestMove = child->lastMove();
                bestMove.setScore(score);
            }

            #pragma omp critical
            alpha = std::max(alpha, bestMove.score());
            // must be continue instead of break bc parallel
        }
    } else {
        bestMove.setScore(eval_constants::MAX_SCORE + 1);

        #pragma omp parallel for
        for (const auto& child : gameNode.children()) {
            if (beta <= alpha) {
                continue;
            }
            auto move = alphaBeta(SequentialTag{}, *child, depth - 1, alpha, beta, true);
            auto score = move.score();
            if (score < bestMove.score()) {
                bestMove = child->lastMove();
                bestMove.setScore(score);
            }

            #pragma omp critical
            beta = std::min(beta, bestMove.score());
        }
    }
    return bestMove;
} // Naive


// clustering TODO
chess::Move alphaBeta(
    const SharedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
) {
    // Verify input parameters
    if (alpha < eval_constants::MIN_SCORE || alpha > eval_constants::MAX_SCORE) {
        throw std::out_of_range("Alpha out of bounds.");
    }
    if (beta < eval_constants::MIN_SCORE || beta > eval_constants::MAX_SCORE) {
        throw std::out_of_range("Beta out of bounds.");
    }

    // TODO: Implementation with local alpha and beta values to avoid critical
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
        bestMove.setScore(eval_constants::MIN_SCORE - 1);

        #pragma omp parallel for
        for (const auto& child : gameNode.children()) {
            auto localAlpha = alpha, localBeta = beta;
            if (localBeta <= localAlpha) {
                continue;
            }
            auto move = alphaBeta(SequentialTag{}, *child, depth - 1, localAlpha, localBeta, false);
            auto score = move.score();
            if (score > bestMove.score()) {
                bestMove = child->lastMove();
                bestMove.setScore(score);
            }

            localAlpha = std::max(localAlpha, bestMove.score());
        }
    } else {
        bestMove.setScore(eval_constants::MAX_SCORE + 1);

        #pragma omp parallel for
        for (const auto& child : gameNode.children()) {
            auto localAlpha = alpha, localBeta = beta;
            if (localBeta <= localAlpha) {
                continue;
            }
            auto move = alphaBeta(SequentialTag{}, *child, depth - 1, localAlpha, localBeta, true);
            auto score = move.score();
            if (score < bestMove.score()) {
                bestMove = child->lastMove();
                bestMove.setScore(score);
            }
            localBeta = std::min(localBeta, bestMove.score());
            // must be continue instead of break bc parallel
        }
    }
    return bestMove;
}
