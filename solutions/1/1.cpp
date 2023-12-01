// File auto-generated by StartNewDay

#include <ACSolver.ipp>
#include <Utilities.ipp>
#include <gtest/gtest.h>
#include <numeric>
#include <ranges>

namespace
{
    namespace part1
    {
        int64_t GetFirst(const std::string& x)
        {
            // TODO: Replace with std::find
            for(unsigned char c : x)
            {
                if(std::isdigit(c) != 0)
                {
                    return util::StringTo<int64_t>(std::string(1, static_cast<char>(c)));
                }
            }

            return {};
        }

        int64_t GetLast(const std::string& x)
        {
            // TODO: Replace with std::find
            for(unsigned char c : std::ranges::reverse_view(x))
            {
                if(std::isdigit(c) != 0)
                {
                    return util::StringTo<int64_t>(std::string(1, static_cast<char>(c)));
                }
            }

            return {};
        }

        int64_t GetCalibrationValue(const std::string& x)
        {
            return GetFirst(x) * 10 + GetLast(x);
        }

    }

    namespace part2
    {
        constexpr std::array<std::pair<const char*, int64_t>, 9> Numbers = {
            std::make_pair("one", 1), std::make_pair("two", 2),   std::make_pair("three", 3), std::make_pair("four", 4), std::make_pair("five", 5),
                                                        std::make_pair("six", 6), std::make_pair("seven", 7), std::make_pair("eight", 8), std::make_pair("nine", 9)};
        using Position = std::pair<std::optional<int64_t>, size_t>;

        int64_t GetFirst(const std::string& x)
        {
            const auto literal = [&x]() -> Position {
                for(auto it = x.begin(); it != x.end(); ++it)
                {
                    if(std::isdigit(*it) != 0)
                    {
                        const auto number = util::StringTo<int64_t>(std::string(1, static_cast<char>(*it)));
                        const auto index = std::distance(x.begin(), it);
                        return {number, index};
                    }
                }

                return {};
            }();

            const auto text = [&x]() -> Position {
                Position p = {std::nullopt, x.size()};
                for(const auto& [text, num] : Numbers) {
                    if(const auto pos = x.find(text); pos != std::string::npos && pos < p.second)
                    {
                        p = {num, pos};
                    }
                }

                return p;
            }();

            if(literal.second < text.second && literal.first.has_value())
            {
                return literal.first.value();
            }

            return text.first.value_or(int64_t{});
        }

        int64_t GetLast(const std::string& x)
        {
            const auto literal = [&x]() -> Position {
                for(auto it = x.rbegin(); it != x.rend(); ++it)
                {
                    if(std::isdigit(*it) != 0)
                    {
                        const auto number = util::StringTo<int64_t>(std::string(1, static_cast<char>(*it)));
                        const auto index = std::distance(x.rbegin(), it);
                        return {number, index};
                    }
                }

                return {};
            }();

            const auto text = [&x]() -> Position {
                Position p = {std::nullopt, x.size()};
                for(const auto& [text, num] : Numbers) {
                    if(const auto pos = x.rfind(text); pos != std::string::npos &&  x.size() - pos < p.second)
                    {
                        p = {num, x.size() - pos};
                    }
                }

                return p;
            }();

            if(literal.second < text.second && literal.first.has_value())
            {
                return literal.first.value();
            }

            return text.first.value_or(int64_t{});
        }

        int64_t GetCalibrationValue(const std::string& x)
        {
            return GetFirst(x) * 10 + GetLast(x);
        }
    }

    struct Solver : public ACSolver
    {
        Answer solvePartOne() override
        {
            std::vector<int64_t> calibrationValues;
            std::transform(mInput.begin(), mInput.end(), std::back_inserter(calibrationValues), &part1::GetCalibrationValue);
            return std::accumulate(calibrationValues.begin(), calibrationValues.end(), 0, std::plus<>());
        }

        Answer solvePartTwo() override
        {
            std::vector<int64_t> calibrationValues;
            std::transform(mInput.begin(), mInput.end(), std::back_inserter(calibrationValues), &part2::GetCalibrationValue);
            return std::accumulate(calibrationValues.begin(), calibrationValues.end(), 0, std::plus<>());
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
    std::cout << "part one: " << *answer << std::endl;
}

TEST(day_1, part_2)
{
    const auto answer = CreateSolver("1.txt").solvePartTwo();
    ASSERT_NE(answer, std::nullopt);
    // EXPECT_EQ(*answer, INSERT_CORRECT_ANSWER);
    std::cout << "part two: " << *answer << std::endl;
}

TEST(day_1, sample_part_1)
{
    const auto answer = CreateSolver("1_sample_part1.txt").solvePartOne();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 142);
    std::cout << "part one sample: " << *answer << std::endl;
}

TEST(day_1, sample_part_2)
{
    const auto answer = CreateSolver("1_sample_part2.txt").solvePartTwo();
    ASSERT_NE(answer, std::nullopt);
    EXPECT_EQ(*answer, 281);
    std::cout << "part two sample: " << *answer << std::endl;
}
