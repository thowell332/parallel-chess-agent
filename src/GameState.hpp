/**
 * @file GameState.hpp
 */

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <cstdint>
#include <chess.hpp>

// Material values of each piece type
constexpr std::int16_t K_WT = 200, Q_WT = 9, R_WT = 5, B_WT = 3, N_WT = 3, P_WT = 1;

// Define bounds for evaluation function
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
         * @brief Accessor for the last move.
         */
        chess::Move lastMove() const { return lastMove_; }

        /**
         * @brief Updates the board state for a given move and stores the move in memory.
         * 
         * @param move Move to execute.
         */
        void makeMove(const chess::Move & move);

        /**
         * @brief Returns true if it is the specified color's turn to move.
         */
        template <chess::Color::underlying color>
        bool isMyTurn() const { return chess::Color(color) == board_.sideToMove(); }

        /**
         * @brief Evaluates the current board position and scores the last move.
         */
        template <chess::Color::underlying color>
        chess::Move lastMoveScored() const {
            std:int16_t activePlayerScore = evaluateBoard();
            std::int16_t myScore = activePlayerScore ? isMyTurn<color>() : -activePlayerScore;
            auto returnMove = lastMove_;
            returnMove.setScore(myScore);
            return returnMove;
        }

    private:
        chess::Board board_;
        chess::Move lastMove_;

        /**
         * @brief Calculates a score for the current board position relative to the active player.
         * 
         * @param board Current board position.
         * 
         * @return Score representing relative strategic advantage of the given board position for the 
         * active player.
         */
        std::int16_t evaluateBoard() const;
}; // class GameState

#endif // GAME_STATE_HPP
