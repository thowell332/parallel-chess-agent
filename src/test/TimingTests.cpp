/**
 * @file TimingTests.cpp
 */

#include "../AlphaBeta.hpp"
#include <chess.hpp>
#include <chrono>

template<typename T>
std::pair<double, chess::Move> depthTimingTest(int trials, int depth)
{
    // the actual starting position
    constexpr auto startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    auto root = std::make_unique<GameNode>(startPos);
    double totalTime = 0; // in microseconds

    chess::Move selectedMove;

    for (int i = 0; i < trials; i++)
    {
        auto start = std::chrono::steady_clock::now();
        selectedMove = alphaBeta(T{}, *root, depth);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        totalTime += (double)duration;
    }

    return std::pair<double, chess::Move>((double)totalTime/trials, selectedMove);
}


constexpr int TRIALS = 3, DEPTH = 6;
int main(int argc, char *argv[])
{
    // TODO: Implement unit tests and timing tests
    auto seqRes = depthTimingTest<SequentialTag>(TRIALS, DEPTH);
    std::cout << "Sequential time: " << seqRes.first << '\n'
              << "Sequential move: " << seqRes.second << std::endl;


    auto naiveRes = depthTimingTest<NaiveSharedMemoryTag>(TRIALS, DEPTH);
    std::cout << "Naive Shmem time: " << naiveRes.first << '\n'
              << "Naive Shmem move: " << naiveRes.second << std::endl;

}
