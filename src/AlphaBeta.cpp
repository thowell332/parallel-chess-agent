/**
 * @file AlphaBeta.cpp
 */

#include "AlphaBeta.hpp"
#include <omp.h>

#include <numeric>

// Combiner expressions for the custom reductions
void combinerMin(const chess::Move& in, chess::Move& out) {
    out = out.score() < in.score() ? out : in;
}

void combinerMax(const chess::Move& in, chess::Move& out) {
    out = out.score() > in.score() ? out : in;
}

// Custom reductions for chess::Move
#pragma omp declare reduction(moveMin : chess::Move : combinerMin(omp_out, omp_in)) initializer(omp_priv = omp_orig)
#pragma omp declare reduction(moveMax : chess::Move : combinerMax(omp_out, omp_in)) initializer(omp_priv = omp_orig)

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
        #pragma omp parallel for shared(bestMove, alpha, beta) reduction(+:nodesExplored)
        for (const auto& child : gameNode.children()) {
            if (beta <= alpha) {
                continue;
            }
            auto result = alphaBeta(policy, *child, depth - 1, alpha, beta, false);
            auto score = result.bestMove.score();
            nodesExplored += result.nodesExplored;
            // Only enter critical sections if there is work to do
            if (score > bestMove.score()) {
                #pragma omp critical
                {
                    if (score > bestMove.score()) {
                        bestMove = child->lastMove();
                        bestMove.setScore(score);
                    }
                } // omp critical
            }
            if (bestMove.score() > alpha) {
                #pragma omp critical
                {
                    alpha = std::max(alpha, bestMove.score());
                } // omp critical
            }
        }
    } else {
        bestMove.setScore(eval_constants::MAX_SCORE + 1);
        #pragma omp parallel for shared(bestMove, alpha, beta) reduction(+:nodesExplored)
        for (const auto& child : gameNode.children()) {
            if (beta <= alpha) {
                continue;
            }
            auto result = alphaBeta(policy, *child, depth - 1, alpha, beta, true);
            auto score = result.bestMove.score();
            nodesExplored += result.nodesExplored;
            // Only enter critical sections if there is work to do
            if (score < bestMove.score()) {
                #pragma omp critical
                {
                    if (score < bestMove.score()) {
                        bestMove = child->lastMove();
                        bestMove.setScore(score);
                    }
                } // omp critical
            }
            if (bestMove.score() < beta) {
                #pragma omp critical
                {
                    beta = std::min(beta, bestMove.score());
                } // omp critical
            }
        } // omp parallel for
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
    #pragma omp firstprivate(alpha, beta, bestMove) reduction(+:nodesExplored)
    {
        if (isMaximizingPlayer) {
            #pragma omp reduction(moveMax : bestMove)
            {
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
                } // omp parallel for
            } // omp reduction max:bestMove
        } else {
            #pragma omp reduction(moveMin : bestMove)
            {
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
                } // omp parallel for
            } // omp reduction min:bestMove
        }
    } // omp firstprivate
    return {bestMove, nodesExplored};
}

AlphaBetaResult alphaBeta(
    const GameNode& gameNode,
    std::uint8_t depth,
    const std::uint8_t numSyncInterations, // number of iterations before sync'ing
    std::uint8_t curSyncIteration,
    std::int16_t *globalAlpha, // globals must be instantiated outside of function call
    std::int16_t *globalBeta,
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

    // every couple iterations sync-up alpha and beta values
    if (curSyncIteration == 0) {
        if (alpha > *globalAlpha) {
            #pragma omp critical
            {
                *globalAlpha = alpha = std::max(alpha, *globalAlpha);
            } // omp critical
        }
        if (beta < *globalBeta) {
            #pragma omp critical
            {
                *globalBeta = beta = std::min(beta, *globalBeta);
            } // omp critical
        }
        curSyncIteration = numSyncInterations;
    }
    else {
        curSyncIteration -= 1;
    }

    chess::Move bestMove;
    size_t nodesExplored = 0;
    #pragma omp firstprivate(alpha, beta, bestMove) reduction(+:nodesExplored)
    {
        if (isMaximizingPlayer) {
            #pragma omp reduction(moveMax : bestMove)
            {
                bestMove.setScore(eval_constants::MIN_SCORE - 1);

                #pragma omp parallel for
                for (const auto& child : gameNode.children()) {
                    if (beta <= alpha) {
                        continue;
                    }

                    auto result = alphaBeta(*child, depth - 1, numSyncInterations, curSyncIteration, globalAlpha, globalBeta, alpha, beta,  false);
                    nodesExplored += result.nodesExplored;
                    auto score = result.bestMove.score();
                    if (score > bestMove.score()) {
                        bestMove = child->lastMove();
                        bestMove.setScore(score);
                    }
                    alpha = std::max(alpha, bestMove.score());
                } // omp parallel for
            } // omp reduction max:bestMove
        } else {
            #pragma omp reduction(moveMin : bestMove)
            {
                bestMove.setScore(eval_constants::MAX_SCORE + 1);
                #pragma omp parallel for
                for (const auto& child : gameNode.children()) {
                    if (beta <= alpha) {
                        continue;
                    }
                    auto result = alphaBeta(*child, depth - 1, numSyncInterations, curSyncIteration - 1, globalAlpha, globalBeta, alpha, beta,  true);
                    nodesExplored += result.nodesExplored;
                    auto score = result.bestMove.score();
                    if (score < bestMove.score()) {
                        bestMove = child->lastMove();
                        bestMove.setScore(score);
                    }
                    beta = std::min(beta, bestMove.score());
                } // omp parallel for
            } // omp reduction min:bestMove
        } // if (maximizingPlayer)
    } // omp firstprivate
    return {bestMove, nodesExplored};
}
