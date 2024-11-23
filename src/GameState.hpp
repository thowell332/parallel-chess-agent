/**
 * @file GameState.hpp
 */

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <cstdint>
#include <chess.hpp>

// material values of each piece type
constexpr int16_t K_WT = 200, Q_WT = 9, R_WT = 5, B_WT = 3,
                    N_WT = 3, P_WT = 1;

// TODO: Define bounding scores for evaluation function
constexpr std::int16_t MAX_SCORE = K_WT + Q_WT + 2*R_WT + 2*B_WT + 2*N_WT + 8*P_WT;
constexpr std::int16_t MIN_SCORE = -MAX_SCORE;

/**
 * @class GameState
 * @brief Describes the current game state by storing the board position and last move.
 */
class GameState
{
    public:
        // Default constructor and copy constructor
        GameState() = default;
        GameState(const GameState&) = default;

        /**
         * @brief Accessor for the current board position.
         */
        chess::Board board() const { return board_; }

        /**
         * @brief Returns a copy of the last move with a score computed as the evaluation score of
         * the current board position.
         */
        chess::Move lastMoveScored() const {
            auto returnMove = lastMove_;
            returnMove.setScore(evaluateBoard());
            return returnMove;
        }

        /**
         * @brief Updates the board state for a given move and stores the move in memory.
         * 
         * @param move Move to execute.
         */
        void makeMove(const chess::Move & move) {
            board_.makeMove(move);
            lastMove_ = move;
        }

    private:
        chess::Board board_;
        chess::Move lastMove_;

        /**
         * @brief Assigns a score to the current board state.
         * 
         * @param board Current board state.
         * 
         * @return Score representing relative strategic advantage of the given board state.
         */
        std::int16_t evaluateBoard() const {
            // actual end game conditions, from current player's perspective
            auto result = board_.isGameOver();
            if (result.second == chess::GameResult::WIN) {
                return MAX_SCORE;
            }
            else if (result.second == chess::GameResult::LOSE) {
                return MIN_SCORE;
            }
            else { // draw
                return 0;
            }

            // heuristic
            int wKings = board_.pieces(chess::PieceType::KING, chess::Color::WHITE).count(),
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

                int16_t materialScore = K_WT * (wKings - bKings) +
                                        Q_WT * (wQueens - bQueens) +
                                        R_WT * (wRooks - bRooks) +
                                        B_WT * (wBishops - bBishops) +
                                        N_WT * (wKnights - bKnights) +
                                        P_WT * (wPawns - bPawns);

            // TODO do mobility score, possibly
            // chess::Movelist mvlist();
            // mobilityScore = chess::movegen::legalmoves(mvlist, board_, );

            int16_t who2move = (board_.sideToMove() == chess::Color::WHITE) ? 1 : -1;
            return materialScore * who2move;
        } //evaluateBoard
};

#endif // GAME_STATE_HPP
