/**
 * @file AlphaBeta.cpp
 */

#include "AlphaBeta.hpp"
#include <omp.h>

#include <numeric>

AlphaBetaResult
alphaBeta(
    const SequentialTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
) {
    // Return if the maximum depth has been explored or there are no legal moves remaining
    if (depth == 0 || gameNode.children().empty()) {
        auto move = gameNode.lastMove();
        auto activePlayerScore = gameNode.evaluateBoard();
        auto score = isMaximizingPlayer ? activePlayerScore : -activePlayerScore;
        move.setScore(score);
        return {move, 1};
    }

    chess::Move bestMove;
    size_t nodesExplored = 0;
    if (isMaximizingPlayer) {
        bestMove.setScore(eval_constants::MIN_SCORE - 1);
        for (const auto& child : gameNode.children()) {
            auto result = alphaBeta(policy, *child, depth - 1, alpha, beta, false);
            nodesExplored += result.nodesExplored;
            auto score = result.bestMove.score();
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
            auto result = alphaBeta(policy, *child, depth - 1, alpha, beta, true);
            nodesExplored += result.nodesExplored;
            auto score = result.bestMove.score();
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
    return {bestMove, nodesExplored};
}

AlphaBetaResult
alphaBeta(
    const NaiveSharedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
) {
    // Return if the maximum depth has been explored or there are no legal moves remaining
    if (depth == 0 || gameNode.children().empty()) {
        auto move = gameNode.lastMove();
        auto activePlayerScore = gameNode.evaluateBoard();
        auto score = isMaximizingPlayer ? activePlayerScore : -activePlayerScore;
        move.setScore(score);
        return {move, 1};
    }

    chess::Move bestMove;
    size_t nodesExplored = 0;
    if (isMaximizingPlayer) {
        bestMove.setScore(eval_constants::MIN_SCORE - 1);
        #pragma omp parallel for shared(bestMove, nodesExplored, alpha, beta)
        for (const auto& child : gameNode.children()) {
            if (beta <= alpha) {
                continue;
            }
            auto result = alphaBeta(policy, *child, depth - 1, alpha, beta, false);
            auto score = result.bestMove.score();
            #pragma omp critical
            {
                nodesExplored += result.nodesExplored;
                if (score > bestMove.score()) {
                    bestMove = child->lastMove();
                    bestMove.setScore(score);
                }
                alpha = std::max(alpha, bestMove.score());
            } // omp critical
        }
    } else {
        bestMove.setScore(eval_constants::MAX_SCORE + 1);
        #pragma omp parallel for shared(bestMove, nodesExplored, alpha, beta)
        for (const auto& child : gameNode.children()) {
            if (beta <= alpha) {
                continue;
            }
            auto result = alphaBeta(policy, *child, depth - 1, alpha, beta, true);
            auto score = result.bestMove.score();
            #pragma omp critical
            {
                nodesExplored += result.nodesExplored;
                if (score < bestMove.score()) {
                    bestMove = child->lastMove();
                    bestMove.setScore(score);
                }
                beta = std::min(beta, bestMove.score());
            } // omp critical
        }
    }
    return {bestMove, nodesExplored};
}

AlphaBetaResult alphaBeta(
    const SharedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
) {
    // Return if the maximum depth has been explored or there are no legal moves remaining
    if (depth == 0 || gameNode.children().empty()) {
        auto move = gameNode.lastMove();
        auto activePlayerScore = gameNode.evaluateBoard();
        auto score = isMaximizingPlayer ? activePlayerScore : -activePlayerScore;
        move.setScore(score);
        return {move, 1};
    }

    chess::Move bestMove;
    size_t nodesExplored = 0;
    #pragma omp firstprivate(alpha, beta)
    {
        if (isMaximizingPlayer) {
            bestMove.setScore(eval_constants::MIN_SCORE - 1);
            #pragma omp parallel for shared(bestMove, nodesExplored)
            for (const auto& child : gameNode.children()) {
                if (beta <= alpha) {
                    continue;
                }
                auto result = alphaBeta(policy, *child, depth - 1, alpha, beta, false);
                auto score = result.bestMove.score();
                alpha = std::max(alpha, bestMove.score());
                #pragma omp critical
                {
                    nodesExplored += result.nodesExplored;
                    if (score > bestMove.score()) {
                        bestMove = child->lastMove();
                        bestMove.setScore(score);
                    }
                } // omp critical
            }
        } else {
            bestMove.setScore(eval_constants::MAX_SCORE + 1);
            #pragma omp parallel for shared(bestMove, nodesExplored)
            for (const auto& child : gameNode.children()) {
                if (beta <= alpha) {
                    continue;
                }
                auto result = alphaBeta(policy, *child, depth - 1, alpha, beta, true);
                auto score = result.bestMove.score();
                beta = std::min(beta, bestMove.score());
                #pragma omp critical
                {
                    nodesExplored += result.nodesExplored;
                    if (score < bestMove.score()) {
                        bestMove = child->lastMove();
                        bestMove.setScore(score);
                    }
                } // omp critical
            }
        }
    }
    return {bestMove, nodesExplored};
}

AlphaBetaResult
alphaBeta(
    const DistributedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
) {
    chess::Move bestMove;
    bestMove.setScore(0);
    size_t nodesExplored = 0;
    //TODO: Implement distributed memory algorithm
    return {bestMove, nodesExplored};
}
