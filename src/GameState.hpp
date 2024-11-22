/**
 * @file GameState.hpp
 */

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <cstdint>
#include <chess.hpp>

// TODO: Define bounding scores for evaluation function
constexpr std::int16_t MIN_SCORE = -100;
constexpr std::int16_t MAX_SCORE = 100;

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
            // TODO: Implement function
            return 0;
        };
};

#endif // GAME_STATE_HPP
