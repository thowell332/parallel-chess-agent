/**
 * @file TimingTests.cpp
 */

#include "../AlphaBeta.hpp"
#include <chess.hpp>

#include <numeric>
#include <chrono>

struct TimingTestResult
{
    using TimeType = std::chrono::microseconds;
    std::vector<TimeType> times;
    std::vector<AlphaBetaResult> results;

    void add(TimeType time, AlphaBetaResult result) {
        times.emplace_back(time);
        results.emplace_back(result);
    }

    double averageTime() const {
        std::vector<double> timeCounts(times.size());
        std::transform(times.begin(), times.end(), timeCounts.begin(),
            [](TimeType x){ return x.count(); });
        return std::accumulate(timeCounts.begin(), timeCounts.end(), 0.0) / times.size();
    }

    size_t averageNodesExplored() const {
        std::vector<size_t> nodeCounts(results.size());
        std::transform(results.begin(), results.end(), nodeCounts.begin(),
            [](AlphaBetaResult x){ return x.nodesExplored; });
        return std::accumulate(nodeCounts.begin(), nodeCounts.end(), 0) / times.size();
    }

    double averageTimePerNode() const {
        if (times.size() != results.size()) {
            throw std::runtime_error("Inconsistent number of trials.");
        }
        double totalTimePerNode = 0;
        for (int trial = 0; trial < times.size(); ++trial) {
            totalTimePerNode += times[trial].count() / results[trial].nodesExplored;
        }
        return totalTimePerNode / times.size();
    }
};

template<typename Tag>
TimingTestResult depthTimingTest(int trials, int depth)
{
    constexpr auto startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    auto root = std::make_unique<GameNode>(startPos);
    TimingTestResult timingTestResult;
    for (int i = 0; i < trials; ++i) {
        auto start = std::chrono::steady_clock::now();
        auto alphaBetaResult = alphaBeta(Tag{}, *root, depth);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        timingTestResult.add(duration, alphaBetaResult);
    }
    return timingTestResult;
}

int main(int argc, char *argv[])
{
    int DEPTH, TRIALS;
    if (argc != 3){
        fprintf(stderr, "Usage: %s DEPTH TRIALS\n", argv[0]);
        exit(EXIT_SUCCESS);
    }
    if (argc >= 2) {
        DEPTH = strtol(argv[1], NULL, 10);
    }
    if (argc >= 3) {
        TRIALS = strtol(argv[1], NULL, 10);
    }

    auto sequentialResult = depthTimingTest<SequentialTag>(TRIALS, DEPTH);
    std::cout << "Sequential results:" << std::endl
              << "Average time (ms):          " << sequentialResult.averageTime()/1e3      << std::endl
              << "Average nodes explored:     " << sequentialResult.averageNodesExplored() << std::endl
              << "Average time per node (us): " << sequentialResult.averageTimePerNode()   << std::endl
              << std::endl;
    auto naiveShmemResult = depthTimingTest<NaiveSharedMemoryTag>(TRIALS, DEPTH);
    std::cout << "Naive shared memory results:" << std::endl
              << "Average time (ms):          " << naiveShmemResult.averageTime()/1e3      << std::endl
              << "Average nodes explored:     " << naiveShmemResult.averageNodesExplored() << std::endl
              << "Average time per node (us): " << naiveShmemResult.averageTimePerNode()   << std::endl
              << std::endl;
    auto shmemResult = depthTimingTest<SharedMemoryTag>(TRIALS, DEPTH);
    std::cout << "Shared memory results:" << std::endl
              << "Average time (ms):          " << shmemResult.averageTime()/1e3      << std::endl
              << "Average nodes explored:     " << shmemResult.averageNodesExplored() << std::endl
              << "Average time per node (us): " << shmemResult.averageTimePerNode()   << std::endl
              << std::endl;
}
