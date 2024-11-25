/**
 * @file GameNode.cpp
 */

#include "GameNode.hpp"

GameNode::GameNode(chess::Board board, chess::Move move)
    : board_(board)
    , lastMove_(move)
    , childrenInitialized_(false)
{
    // Execute move on the board position of the parent node
    board_.makeMove(move);
}

const std::vector<std::unique_ptr<GameNode>>&
GameNode::children() const
{
    // Only construct child nodes if they have not already been initialized
    if (!childrenInitialized_) {
        // Generate all legal moves and construct child nodes
        chess::Movelist movelist;
        chess::movegen::legalmoves(movelist, board_);
        for (const auto& nextMove : movelist) {
            children_.emplace_back(std::make_unique<GameNode>(board_, nextMove));
        }
        childrenInitialized_ = true;
    }
    return children_;
}

std::int16_t
GameNode::evaluateBoard() const
{
    // Evaluate end game conditions relative to the active player
    auto result = board_.isGameOver();
    if (result.second == chess::GameResult::WIN)  return MAX_SCORE;
    if (result.second == chess::GameResult::LOSE) return MIN_SCORE;
    if (result.second == chess::GameResult::DRAW) return 0;

    // Compute material score
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
