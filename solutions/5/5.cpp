// File auto-generated by StartNewDay

#include <ACSolver.ipp>
#include <Utilities.ipp>
#include <algorithm>
#include <gtest/gtest.h>

namespace
{
    struct Farm
    {
        // GeneratorMap?
        // Maybe more complicated than using a regular map
        struct GeneratorMap
        {
            GeneratorMap(int64_t source, int64_t destination, int64_t range)
                : mSource(source), mDestination(destination), mRange(range)
            {
            }

            bool contains(int64_t source) const
            {
                return source >= mSource && source <= mSource + mRange;
            }

            std::optional<int64_t> get(int64_t source) const
            {
                if(contains(source))
                {
                    return source + (mDestination - mSource);
                }

                return {};
            }

            int64_t mSource;
            int64_t mDestination;
            int64_t mRange;
        };

        struct Garden
        {
            void addMap(int64_t source, int64_t destination, int64_t range)
            {
                mMaps.emplace_back(source, destination, range);
            }

            bool contains(int64_t source) const
            {
                return std::any_of(mMaps.begin(), mMaps.end(),
                                   [source](const GeneratorMap& map)
                                   { return map.contains(source); });
            }

            int64_t get(int64_t source) const
            {
                const auto it = std::find_if(mMaps.begin(), mMaps.end(),
                                             [source](const GeneratorMap& map)
                                             { return map.contains(source); });
                if(it != mMaps.end())
                {
                    return it->get(source).value();
                }

                return source;
            }

            std::vector<GeneratorMap> mMaps;
        };

        static Farm Build(const std::vector<std::string>& x)
        {
            Farm farm;

            {
                const auto& seeds = x.front();
                const auto tokens = util::Split(seeds.substr(seeds.find(':') + 1));
                std::transform(tokens.begin(), tokens.end(), std::back_inserter(farm.mSeeds),
                               [](const std::string& token)
                               { return util::StringTo<int64_t>(token); });
            }

            // seed-to-soil map:
            // 50 98 2
            // 52 50 48
            for(const auto& line : x)
            {
                if(line.find("map:") != std::string::npos)
                {
                    farm.mGardens.emplace_back();
                }
                else if(!line.empty() && !farm.mGardens.empty())
                {
                    // DESTINATION SOURCE RANGE
                    const auto tokens = util::Split(line);
                    const auto source = util::StringTo<int64_t>(tokens[1]);
                    const auto destination = util::StringTo<int64_t>(tokens[0]);
                    const auto range = util::StringTo<int64_t>(tokens[2]);

                    auto& garden = farm.mGardens.back();
                    garden.addMap(source, destination, range);
                }
            }

            return farm;
        }

        int64_t getLocationNumber(int64_t seed) const
        {
            auto locationNumber = seed;
            for(const auto& garden : mGardens)
            {
                locationNumber = garden.get(locationNumber);
            }

            return locationNumber;
        }

        std::vector<int64_t> mSeeds;
        std::vector<Garden> mGardens;
    };

    struct Solver : public ACSolver
    {
        Answer solvePartOne() override
        {
            const auto f = Farm::Build(mInput);

            using SeedLocation = std::pair<int64_t, int64_t>;
            std::vector<SeedLocation> seedLocations;
            std::transform(f.mSeeds.begin(), f.mSeeds.end(), std::back_inserter(seedLocations),
                           [&f](int64_t seed)
                           { return std::make_pair(f.getLocationNumber(seed), seed); });
            std::sort(seedLocations.begin(), seedLocations.end(),
                      [](const SeedLocation first, const SeedLocation second)
                      { return first.first < second.first; });

            return seedLocations.front().first;
        }

        Answer solvePartTwo() override
        {
            const auto f = Farm::Build(mInput);

            using SeedLocation = std::pair<int64_t, int64_t>;
            SeedLocation lowestSeedLocation{std::numeric_limits<int64_t>::max(), 0};
            for(auto i = 0; i < f.mSeeds.size() - 1; i += 2)
            {
                const auto start = f.mSeeds[i];
                const auto range = f.mSeeds[i + 1];

                std::cout << "Checking " << start << ", " << range << ":\n";

                // This loop is super slow. I had to add print statements just to make sure progress
                // was being made.
                for(auto j = 0; j < range; ++j)
                {
                    const auto seed = start + j;
                    const auto seedLocation = f.getLocationNumber(seed);
                    if(seedLocation < lowestSeedLocation.first)
                    {
                        lowestSeedLocation = {seedLocation, seed};

                        std::cout << "\t Updated: " << seed << ", " << seedLocation << "\n";
                    }
                }
            }

            return lowestSeedLocation.first;
        }
    };

    Solver CreateSolver(bool useSample)
    {
        const auto inputFile = useSample ? "5_sample.txt" : "5.txt";
        Solver solver;
        solver.mInput = util::Parse(util::GetInputFile(inputFile));
        return solver;
    }
}

TEST(day_5, part_1)
{
    const auto answer = CreateSolver(false).solvePartOne();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 535088217);
    std::cout << "[::::::::::] " << *answer << std::endl;
}

TEST(day_5, part_2)
{
    const auto answer = CreateSolver(false).solvePartTwo();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 51399228);
    std::cout << "[::::::::::] " << *answer << std::endl;
}

TEST(day_5, sample_part_1)
{
    const auto answer = CreateSolver(true).solvePartOne();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 35);
    std::cout << "[::::::::::] " << *answer << std::endl;
}

TEST(day_5, sample_part_2)
{
    const auto answer = CreateSolver(true).solvePartTwo();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 46);
    std::cout << "[::::::::::] " << *answer << std::endl;
}
