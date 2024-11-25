/**
 * @file GameNode.hpp
 */

#ifndef GAME_NODE_HPP
#define GAME_NODE_HPP

#include <chess.hpp>

#include <cstdint>
#include <memory>
#include <string_view>

namespace EvalConstants {
    // Material values of each piece type
    constexpr std::int16_t K_WT = 200, Q_WT = 9, R_WT = 5, B_WT = 3, N_WT = 3, P_WT = 1;

    // Define bounds for evaluation function
    constexpr std::int16_t MAX_SCORE = K_WT + Q_WT + 2*R_WT + 2*B_WT + 2*N_WT + 8*P_WT;
    constexpr std::int16_t MIN_SCORE = -MAX_SCORE;
} // namespace EvalConstants

/**
 * @class GameNode
 * @brief Represents a node in the game tree by storing the board position, last move, and a vector
 * of child nodes constructed from the set of legal moves to be considered from this node.
 */
class GameNode
{
    public:
        // Delete copy constructor and assignment operator
        GameNode(const GameNode&) = delete;
        GameNode& operator=(const GameNode&) = delete;

        /**
         * @brief Constructor used only for the root node of a new game tree.
         * 
         * @param fen FEN string representation of the desired starting board position.
         */
        GameNode(std::string_view fen = chess::constants::STARTPOS)
            : board_(fen), lastMove_(), childrenInitialized_(false) {}

        /**
         * @brief Constructor for all non-root nodes. 
         * 
         * @param board Board position of the parent node.
         * @param move Move to execute and store in this child node.
         */
        GameNode(chess::Board board, chess::Move move);

        /**
         * @brief Default destructor.
         */
        ~GameNode() = default;

        /**
         * @brief Accessor for the current board position.
         */
        const chess::Board& board() const { return board_; }

        /**
         * @brief Accessor for the last move.
         */
        const chess::Move& lastMove() const { return lastMove_; }

        /**
         * @brief Accessor for number of child nodes with lazy initialization. Child nodes are only
         * initialized the first time that this accessor is called.
         */
        const std::vector<std::unique_ptr<GameNode>>& children() const;
        
        /**
         * @brief Execute given move on the current board position and store last move.
         */
        void makeMove(const chess::Move& move);

        /**
         * @brief Calculates a score for the current board position relative to the active player.
         */
        std::int16_t evaluateBoard() const;

    private:
        chess::Board board_;
        chess::Move lastMove_;
        mutable bool childrenInitialized_;
        mutable std::vector<std::unique_ptr<GameNode>> children_;
}; // class GameNode

#endif // GAME_NODE_HPP
