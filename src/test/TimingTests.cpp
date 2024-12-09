/**
 * @file TimingTests.cpp
 */

#include "../AlphaBeta.hpp"
#include <chess.hpp>

#include <algorithm>
#include <chrono>
#include <numeric>

using TimeType = std::chrono::microseconds;

struct TimingTestResult
{
    std::chrono::microseconds duration;
    AlphaBetaResult result;

    double timeAsDouble() const { return duration.count(); }
    size_t nodesExplored() const { return result.nodesExplored; }
};

template<typename Tag>
TimingTestResult depthTimingTest(int depth, const std::string& startPos)
{
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
    size_t posIdx = 0;
    const std::vector<std::string> startPos = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "4B3/K1N1r3/1P3B2/6P1/P7/2R5/3k4/8 w - - 0 1"
    };
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
    if (argc > 2) {
        try {
            posIdx = std::stoi(argv[2]);
            posIdx = std::max(posIdx, startPos.size() - 1);
        } catch (const std::invalid_argument & e) {
            std::cerr << "std::invalid_argument::what() " << e.what() << std::endl;
        } catch (const std::out_of_range & e) {
            std::cerr << "std::out_of_range::what() " << e.what() << std::endl;
        }
    }
    auto shemeResult = depthTimingTest<SharedMemoryTag>(depth, startPos[posIdx]);
    std::cout << shemeResult.timeAsDouble()
              << "," << shemeResult.nodesExplored()
              << std::endl;
}
