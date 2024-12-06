/**
 * @file AlphaBeta.cpp
 */

#include "AlphaBeta.hpp"
#include <omp.h>

AlphaBetaResult
alphaBetaRecurse(
    const SequentialTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
) {
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
alphaBetaRecurse(
    const NaiveSharedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
) {
    chess::Move bestMove;
    size_t nodesExplored = 0;
    if (isMaximizingPlayer) {
        bestMove.setScore(eval_constants::MIN_SCORE - 1);
        #pragma omp parallel for
        for (const auto& child : gameNode.children()) {
            if (beta <= alpha) {
                continue;
            }
            auto result = alphaBeta(policy, *child, depth - 1, alpha, beta, false);
            nodesExplored += result.nodesExplored;
            auto score = result.bestMove.score();
            if (score > bestMove.score()) {
                bestMove = child->lastMove();
                bestMove.setScore(score);
            }
            #pragma omp critical
            alpha = std::max(alpha, bestMove.score());
        }
    } else {
        bestMove.setScore(eval_constants::MAX_SCORE + 1);
        #pragma omp parallel for
        for (const auto& child : gameNode.children()) {
            if (beta <= alpha) {
                continue;
            }
            auto result = alphaBeta(policy, *child, depth - 1, alpha, beta, true);
            nodesExplored += result.nodesExplored;
            auto score = result.bestMove.score();
            if (score < bestMove.score()) {
                bestMove = child->lastMove();
                bestMove.setScore(score);
            }
            #pragma omp critical
            beta = std::min(beta, bestMove.score());
        }
    }
    return {bestMove, nodesExplored};
}

AlphaBetaResult
alphaBetaRecurse(
    const SharedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
) {
    chess::Move bestMove;
    size_t nodesExplored = 0;
    #pragma omp firstprivate(alpha, beta)
    {
        if (isMaximizingPlayer) {
            bestMove.setScore(eval_constants::MIN_SCORE - 1);
            #pragma omp parallel for
            for (const auto& child : gameNode.children()) {
                if (beta <= alpha) {
                    continue;
                }
                auto result = alphaBeta(policy, *child, depth - 1, alpha, beta, false);
                nodesExplored += result.nodesExplored;
                auto score = result.bestMove.score();
                if (score > bestMove.score()) {
                    bestMove = child->lastMove();
                    bestMove.setScore(score);
                }
                alpha = std::max(alpha, bestMove.score());
            }
        } else {
            bestMove.setScore(eval_constants::MAX_SCORE + 1);
            #pragma omp parallel for
            for (const auto& child : gameNode.children()) {
                if (beta <= alpha) {
                    continue;
                }
                auto result = alphaBeta(policy, *child, depth - 1, alpha, beta, true);
                nodesExplored += result.nodesExplored;
                auto score = result.bestMove.score();
                if (score < bestMove.score()) {
                    bestMove = child->lastMove();
                    bestMove.setScore(score);
                }
                beta = std::min(beta, bestMove.score());
            }
        }
    }
    return {bestMove, nodesExplored};
}

AlphaBetaResult
alphaBetaRecurse(
    const DistributedMemoryTag& policy,
    const GameNode& gameNode,
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta,
    bool isMaximizingPlayer
) {
    chess::Move bestMove;
    size_t nodesExplored = 0;
    //TODO: Implement distributed memory algorithm
    return {bestMove, nodesExplored};
}
