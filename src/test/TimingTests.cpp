/**
 * @file TimingTests.cpp
 */

#include "../AlphaBeta.hpp"
#include <chess.hpp>

#include <numeric>
#include <chrono>

using TimeType = std::chrono::microseconds;

struct TimingTestResult
{
    std::chrono::microseconds duration;
    AlphaBetaResult result;

    double timeAsDouble() const { return duration.count(); }
    size_t nodesExplored() const { return result.nodesExplored; }
};

template<typename Tag>
TimingTestResult depthTimingTest(int depth)
{
    constexpr auto startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    auto root = std::make_unique<GameNode>(startPos);

    auto start = std::chrono::steady_clock::now();
    auto result = alphaBeta(Tag{}, *root, depth);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    return {duration, result};
}

int main(int argc, char *argv[])
{
    int depth = 5;
    // Parse command-line argument for depth if one is given
    if (argc > 1) {
        try {
            depth = std::stoi(argv[1]);
        } catch (const std::invalid_argument & e) {
            std::cerr << "std::invalid_argument::what() " << e.what() << std::endl;
        } catch (const std::out_of_range & e) {
            std::cerr << "std::out_of_range::what() " << e.what() << std::endl;
        }
    }
    auto naiveShmemResult = depthTimingTest<NaiveSharedMemoryTag>(depth);
    std::cout << naiveShmemResult.timeAsDouble()
              << "," << naiveShmemResult.nodesExplored()
              << std::endl;
}
