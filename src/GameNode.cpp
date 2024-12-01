/**
 * @file GameNode.cpp
 */

#include "GameNode.hpp"

GameNode::GameNode(chess::Board board, chess::Move move)
    : board_(board)
    , childrenInitialized_(false)
{
    // Execute move on the board position of the parent node
    makeMove(move);
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

void
GameNode::makeMove(const chess::Move& move)
{
    // Update board position and last move
    board_.makeMove(move);
    lastMove_ = move;

    // Clear children and set flag back to false
    childrenInitialized_ = false;
    children_.clear();
}

std::int16_t
GameNode::evaluateBoard() const
{
    // Evaluate end game conditions relative to the active player
    auto result = board_.isGameOver();
    if (result.second == chess::GameResult::WIN)  return eval_constants::MAX_SCORE;
    if (result.second == chess::GameResult::LOSE) return eval_constants::MIN_SCORE;
    if (result.second == chess::GameResult::DRAW) return 0;

    // Number of pieces for each color
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

    // Compute material score for white
    std::int16_t materialScore =
        eval_constants::K_WT * (wKings - bKings) +
        eval_constants::Q_WT * (wQueens - bQueens) +
        eval_constants::R_WT * (wRooks - bRooks) +
        eval_constants::B_WT * (wBishops - bBishops) +
        eval_constants::N_WT * (wKnights - bKnights) +
        eval_constants::P_WT * (wPawns - bPawns);

    // TODO do mobility score, possibly
    // chess::Movelist mvlist();
    // mobilityScore = chess::movegen::legalmoves(mvlist, board_, );

    // Negate material score if black to move
    std::int16_t whiteToMove = (board_.sideToMove() == chess::Color::WHITE) ? 1 : -1;
    return materialScore * whiteToMove;
}
