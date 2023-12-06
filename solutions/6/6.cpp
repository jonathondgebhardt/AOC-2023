// File auto-generated by StartNewDay

#include <ACSolver.ipp>
#include <Utilities.ipp>
#include <gtest/gtest.h>
#include <numeric>

namespace
{
    struct Solver : public ACSolver
    {
        struct RaceRecord
        {
            RaceRecord(size_t time, size_t distance) : mTime(time), mDistance(distance)
            {
            }

            size_t mTime{0};
            size_t mDistance{0};
        };

        static size_t GetWaysToWin(RaceRecord rr)
        {
            const auto getDistanceTraveled = [](size_t buttonHeldTime, RaceRecord rr)
            { return buttonHeldTime * (rr.mTime - buttonHeldTime); };

            size_t lowerBound = 1;
            for(; lowerBound < rr.mTime; ++lowerBound)
            {
                if(getDistanceTraveled(lowerBound, rr) > rr.mDistance)
                {
                    break;
                }
            }

            size_t upperBound = rr.mTime - 1;
            for(; upperBound > lowerBound; --upperBound)
            {
                if(getDistanceTraveled(upperBound, rr) > rr.mDistance)
                {
                    break;
                }
            }

            // Return inclusive range as a sum
            return upperBound - lowerBound + 1;
        }

        Answer solvePartOne() override
        {
            std::vector<RaceRecord> records;

            const auto getTokens = [](const std::string& line)
            {
                const auto colon = line.find(':');
                assert(colon != std::string::npos);

                return util::Split(line.substr(colon + 1));
            };

            const auto& times = getTokens(mInput.front());
            const auto& distances = getTokens(mInput.back());
            assert(times.size() == distances.size());

            for(auto i = 0; i < times.size(); ++i)
            {
                const auto time = util::StringTo<size_t>(times[i]);
                const auto distance = util::StringTo<size_t>(distances[i]);
                records.emplace_back(time, distance);
            }

            return std::accumulate(records.begin(), records.end(), 1,
                                   [](int64_t product, RaceRecord rr)
                                   { return product * GetWaysToWin(rr); });
        }

        Answer solvePartTwo() override
        {
            const auto getTokens = [](const std::string& line)
            {
                const auto colon = line.find(':');
                assert(colon != std::string::npos);

                return util::Split(line.substr(colon + 1));
            };

            const auto& times = getTokens(mInput.front());
            const auto& distances = getTokens(mInput.back());
            assert(times.size() == distances.size());

            std::stringstream time;
            std::stringstream distance;
            for(auto i = 0; i < times.size(); ++i)
            {
                time << util::StringTo<size_t>(times[i]);
                distance << util::StringTo<size_t>(distances[i]);
            }

            RaceRecord rr{util::StringTo<size_t>(time.str()),
                          util::StringTo<size_t>(distance.str())};

            return GetWaysToWin(rr);
        }
    };

    Solver CreateSolver(bool useSample)
    {
        const auto inputFile = useSample ? "6_sample.txt" : "6.txt";
        Solver solver;
        solver.mInput = util::Parse(util::GetInputFile(inputFile));
        return solver;
    }
}

TEST(day_6, part_1)
{
    const auto answer = CreateSolver(false).solvePartOne();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 588588);
    std::cout << "[::::::::::] " << *answer << std::endl;
}

TEST(day_6, part_2)
{
    const auto answer = CreateSolver(false).solvePartTwo();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 34655848);
    std::cout << "[::::::::::] " << *answer << std::endl;
}

TEST(day_6, sample_part_1)
{
    const auto answer = CreateSolver(true).solvePartOne();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 288);
    std::cout << "[::::::::::] " << *answer << std::endl;
}

TEST(day_6, sample_part_2)
{
    const auto answer = CreateSolver(true).solvePartTwo();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 71503);
    std::cout << "[::::::::::] " << *answer << std::endl;
}
