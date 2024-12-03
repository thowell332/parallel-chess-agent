#include "../AlphaBeta.hpp"
#include <chess.hpp>

#include <chrono>

template<typename T>
std::pair<double, chess::Move> timingTest(int trials, int depth)
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


constexpr int TRIALS = 1, DEPTH = 5;
int main(int argc, char *argv[])
{
    // TODO: Implement unit tests and timing tests
    auto seq = timingTest<SequentialTag>(TRIALS, DEPTH);
    std::cout << "Sequential time: " << seq.first << '\n'
              << "Sequential move: " << seq.second << std::endl;


    auto naive = timingTest<NaiveSharedMemoryTag>(TRIALS, DEPTH);
    std::cout << "Naive Shmem time: " << naive.first << '\n'
              << "Naive Shmem move: " << naive.second << std::endl;

}
