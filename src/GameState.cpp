/**
 * @file GameState.cpp
 */

#include "GameState.hpp"

void
GameState::makeMove(const chess::Move & move) {
    board_.makeMove(move);
    lastMove_ = move;
}

std::int16_t
GameState::evaluateBoard() const {
    // actual end game conditions, from current player's perspective
    auto result = board_.isGameOver();
    if (result.second == chess::GameResult::WIN) {
        return MAX_SCORE;
    }
    else if (result.second == chess::GameResult::LOSE) {
        return MIN_SCORE;
    }
    else if (result.second == chess::GameResult::DRAW) {
        return 0;
    }

    // heuristic
    std::int16_t wKings = board_.pieces(chess::PieceType::KING, chess::Color::WHITE).count(),
        bKings = board_.pieces(chess::PieceType::KING, chess::Color::BLACK).count(),
        wQueens = board_.pieces(chess::PieceType::QUEEN, chess::Color::WHITE).count(),
        bQueens = board_.pieces(chess::PieceType::QUEEN, chess::Color::BLACK).count(),
        wRooks = board_.pieces(chess::PieceType::ROOK, chess::Color::WHITE).count(),
        bRooks = board_.pieces(chess::PieceType::ROOK, chess::Color::BLACK).count(),
        wBishops = board_.pieces(chess::PieceType::BISHOP, chess::Color::WHITE).count(),
        bBishops = board_.pieces(chess::PieceType::BISHOP, chess::Color::BLACK).count(),
        wKnights = board_.pieces(chess::PieceType::KNIGHT, chess::Color::WHITE).count(),
        bKnights = board_.pieces(chess::PieceType::KNIGHT, chess::Color::BLACK).count(),
        wPawns = board_.pieces(chess::PieceType::PAWN, chess::Color::WHITE).count(),
        bPawns = board_.pieces(chess::PieceType::PAWN, chess::Color::BLACK).count();

    std::int16_t materialScore =
        K_WT * (wKings - bKings) +
        Q_WT * (wQueens - bQueens) +
        R_WT * (wRooks - bRooks) +
        B_WT * (wBishops - bBishops) +
        N_WT * (wKnights - bKnights) +
        P_WT * (wPawns - bPawns);

    // TODO do mobility score, possibly
    // chess::Movelist mvlist();
    // mobilityScore = chess::movegen::legalmoves(mvlist, board_, );

    std::int16_t whiteToMove = (board_.sideToMove() == chess::Color::WHITE) ? 1 : -1;
    return materialScore * whiteToMove;
}
