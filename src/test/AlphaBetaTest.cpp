/**
 * @file AlphaBetaTest.cpp
 * @brief Implements unit tests and timing tests for minimax algorithm with alpha-beta pruning.
 */

#include "../AlphaBeta.hpp"
#include <chess.hpp>

#include <memory>

/**
 * @brief Executes unit tests to validate correctness of minimax algorithm.
 * 
 * @return Number of failures.
 */
int testCorrectness()
{
    int numTests(0), failures(0);
    std::cout << "Testing checkmate in one..." << std::endl;
    {
        /*
        . k . . . . . .
        . . . . . . R .
        . K . . . . . .
        . . . . . . . .
        . . . . . . . .
        . . . . . . . .
        . . . . . . . .
        . . . . . . . .
        */
        constexpr auto startPos = "1k6/6R1/1K6/8/8/8/8/8 w - - 0 1";
        auto root = std::make_unique<GameNode>(startPos);
        auto selectedMove = alphaBetaSeq<chess::Color::WHITE>(*root, 1);
        // White to move: Rook to g8
        auto bestMove = chess::Move::make(chess::Square("g7"), chess::Square("g8"));
        if (selectedMove == bestMove) {
            std::cout << "----- PASSED -----" << std::endl;
        } else {
            std::cout << "----- FAILED -----" << std::endl;
            std::cout << "Expected: " << bestMove << ". Got " << selectedMove << std::endl;
            std::cout << root->board() << std::endl;
            ++failures;
        }
        ++numTests;
    } {
        /*
        . . . . . . . .
        . . . . . . . .
        . . . . . . . .
        . . . . . . . .
        . . . . . . . .
        . k . . . . . .
        . . . . . . r .
        . K . . . . . .
        */
        constexpr auto startPos = "8/8/8/8/8/1k6/6r1/1K6 b - - 0 1";
        auto root = std::make_unique<GameNode>(startPos);
        auto selectedMove = alphaBetaSeq<chess::Color::BLACK>(*root, 1);
        // Black to move: Rook to g1
        auto bestMove = chess::Move::make(chess::Square("g2"), chess::Square("g1"));
        if (selectedMove == bestMove) {
            std::cout << "----- PASSED -----" << std::endl;
        } else {
            std::cout << "----- FAILED -----" << std::endl;
            std::cout << "Expected: " << bestMove << ". Got " << selectedMove << std::endl;
            std::cout << root->board() << std::endl;
            ++failures;
        }
        ++numTests;
    } {
        /*
        . B b . . . B N
        R . . P k . . r
        . Q . . . . . B
        . . . . q . . R
        . . b N . . . .
        . . . . Q . B K
        . p . . . . . .
        . b q . R . r b
        */
        constexpr auto startPos = "1Bb3BN/R2Pk2r/1Q5B/4q2R/2bN4/4Q1BK/1p6/1bq1R1rb w - - 0 1";
        auto root = std::make_unique<GameNode>(startPos);
        auto selectedMove = alphaBetaSeq<chess::Color::WHITE>(*root, 1);
        // White to move: Queen to a3
        auto bestMove = chess::Move::make(chess::Square("e3"), chess::Square("a3"));
        if (selectedMove == bestMove) {
            std::cout << "----- PASSED -----" << std::endl;
        } else {
            std::cout << "----- FAILED -----" << std::endl;
            std::cout << "Expected: " << bestMove << ". Got " << selectedMove << std::endl;
            std::cout << root->board() << std::endl;
            ++failures;
        }
        ++numTests;
    }
    std::cout << "Testing checkmate in two..." << std::endl;
    {
        /*
        . . . . . Q . .
        p . r . . . . .
        . . . . . . K .
        R . . . . . . .
        . . . . . . k .
        P . . . . . . .
        . . . . . . . .
        . . . . . . . .
        */
        constexpr auto startPos = "5Q2/p1r5/6K1/R7/6k1/P7/8/8 w - - 0 1";
        auto root = std::make_unique<GameNode>(startPos);
        auto selectedMove = alphaBetaSeq<chess::Color::WHITE>(*root, 3);
        // White to move: Rook to g5
        auto bestMove = chess::Move::make(chess::Square("a5"), chess::Square("g5"));
        if (selectedMove == bestMove) {
            std::cout << "----- PASSED -----" << std::endl;
        } else {
            std::cout << "----- FAILED -----" << std::endl;
            std::cout << "Expected: " << bestMove << ". Got " << selectedMove << std::endl;
            std::cout << root->board() << std::endl;
            ++failures;
        }
        ++numTests;
        root->makeMove(selectedMove);
        selectedMove = alphaBetaSeq<chess::Color::BLACK>(*root, 2);
        // Black to move: Anywhere
        root->makeMove(selectedMove);
        selectedMove = alphaBetaSeq<chess::Color::WHITE>(*root, 1);
        // White to move: Queen to h6
        bestMove = chess::Move::make(chess::Square("f8"), chess::Square("h6"));
        if (selectedMove == bestMove) {
            std::cout << "----- PASSED -----" << std::endl;
        } else {
            std::cout << "----- FAILED -----" << std::endl;
            std::cout << "Expected: " << bestMove << ". Got " << selectedMove << std::endl;
            std::cout << root->board() << std::endl;
            ++failures;
        }
        ++numTests;
    }
    return failures;
}

int main(int argc, char* argv[])
{
    // TODO: Implement unit tests and timing tests
    auto failures = testCorrectness();
    return 0;
}
