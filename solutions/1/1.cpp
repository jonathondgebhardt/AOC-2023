// File auto-generated by StartNewDay

#include <ACSolver.ipp>
#include <Utilities.ipp>
#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include <numeric>
#include <ranges>

namespace
{
    std::array<std::pair<std::string, int64_t>, 9> Numbers = {
        std::make_pair("one", 1),   std::make_pair("two", 2),   std::make_pair("three", 3),
        std::make_pair("four", 4),  std::make_pair("five", 5),  std::make_pair("six", 6),
        std::make_pair("seven", 7), std::make_pair("eight", 8), std::make_pair("nine", 9)};

    std::vector<std::pair<size_t, int64_t>> GetNumbers(const std::string& x)
    {
        std::vector<std::pair<size_t, int64_t>> numbers;

        for(auto i = 0; i < x.size(); ++i)
        {
            if(std::isdigit(static_cast<unsigned char>(x[i])) != 0)
            {
                const auto number =
                    util::StringTo<int64_t>(std::string(1, static_cast<char>(x[i])));
                numbers.emplace_back(i, number);
            }
        }

        return numbers;
    }

    std::vector<std::pair<size_t, int64_t>> GetTextNumbers(const std::string& x)
    {
        std::vector<std::pair<size_t, int64_t>> numbers;

        for(auto i = 0; i < x.size(); ++i)
        {
            for(const auto& [text, value] : Numbers)
            {
                if(x.substr(i, text.size()) == text)
                {
                    numbers.emplace_back(i, value);
                    break;
                }
            }
        }

        return numbers;
    }

    namespace part1
    {
        int64_t GetCalibrationValue(const std::string& x)
        {
            const auto numbers = ::GetNumbers(x);

            const auto tens = [&]() -> int64_t
            {
                if(!numbers.empty())
                {
                    return numbers.front().second * 10;
                }

                return 0;
            }();

            const auto ones = [&]() -> int64_t
            {
                if(numbers.size() == 1)
                {
                    return numbers.front().second;
                }
                else if(numbers.size() >= 2)
                {
                    return numbers.back().second;
                }

                return 0;
            }();

            return tens + ones;
        }
    }

    namespace part2
    {
        static int64_t GetCalibrationValue(const std::string& x)
        {
            auto numbers = ::GetNumbers(x);
            const auto textNumbers = ::GetTextNumbers(x);
            std::copy(textNumbers.begin(), textNumbers.end(), std::back_inserter(numbers));
            std::sort(
                numbers.begin(), numbers.end(),
                [](const std::pair<size_t, int64_t> first, const std::pair<size_t, int64_t> second)
                { return first.first < second.first; });

            const auto tens = [&]() -> int64_t
            {
                if(!numbers.empty())
                {
                    return numbers.front().second * 10;
                }

                return 0;
            }();

            const auto ones = [&]() -> int64_t
            {
                if(numbers.size() == 1)
                {
                    return numbers.front().second;
                }
                else if(numbers.size() >= 2)
                {
                    return numbers.back().second;
                }

                return 0;
            }();

            return tens + ones;
        }
    }

    struct Solver : public ACSolver
    {
        Answer solvePartOne() override
        {
            std::vector<int64_t> calibrationValues;
            std::transform(mInput.begin(), mInput.end(), std::back_inserter(calibrationValues),
                           &part1::GetCalibrationValue);
            return std::accumulate(calibrationValues.begin(), calibrationValues.end(), int64_t{0},
                                   std::plus<>());
        }

        Answer solvePartTwo() override
        {
            std::vector<int64_t> calibrationValues;
            std::transform(mInput.begin(), mInput.end(), std::back_inserter(calibrationValues),
                           &part2::GetCalibrationValue);
            return std::accumulate(calibrationValues.begin(), calibrationValues.end(), int64_t{0},
                                   std::plus<>());
        }
    };

    Solver CreateSolver(const std::string& inputFile)
    {
        Solver solver;
        solver.mInput = util::Parse(util::GetInputFile(inputFile));
        return solver;
    }
}

TEST(day_1, part_1)
{
    const auto answer = CreateSolver("1.txt").solvePartOne();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 55971);
    std::cout << "[::::::::::] " << *answer << std::endl;
}

TEST(day_1, part_2)
{
    const auto answer = CreateSolver("1.txt").solvePartTwo();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 54719);
    std::cout << "[::::::::::] " << *answer << std::endl;
}

TEST(day_1, sample_part_1)
{
    const auto answer = CreateSolver("1_sample_part1.txt").solvePartOne();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 142);
    std::cout << "[::::::::::] " << *answer << std::endl;
}

TEST(day_1, sample_part_2)
{
    const auto answer = CreateSolver("1_sample_part2.txt").solvePartTwo();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 281);
    std::cout << "[::::::::::] " << *answer << std::endl;
}
