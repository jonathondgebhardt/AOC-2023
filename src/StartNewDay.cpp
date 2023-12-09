#include "HttpsRequest.hpp"
#include "InputDirectoryConfig.hpp"
#include <cxxopts.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>

std::string DAY;
std::string YEAR;
bool FORCE_OVERWRITE{false};
std::vector<std::string> CREATED_FILES;

bool DayIsValid()
{
    try
    {
        const auto day = std::stoi(DAY);
        return day >= 1 && day <= 31;
    }
    catch(...)
    {
    }

    std::cerr << "Error: '" << DAY << "' must be between 1 and 31 inclusive\n";

    return false;
}

int GetCurrentYear()
{
    // https://stackoverflow.com/a/58153628
    std::time_t t = std::time(nullptr);
    std::tm* const pTInfo = std::localtime(&t);

    auto currentYear = 1900 + pTInfo->tm_year;

    // AoC starts December 1st. If it's not December yet, use the previous year.
    if(pTInfo->tm_mon < 11)
    {
        --currentYear;
    }

    return currentYear;
}

std::string GetCurrentYearString()
{
    return std::to_string(GetCurrentYear());
}

int GetCurrentDay()
{
    // https://stackoverflow.com/a/58153628
    std::time_t t = std::time(nullptr);
    std::tm* const pTInfo = std::localtime(&t);
    return pTInfo->tm_mday;
}

std::string GetCurrentDayString()
{
    return std::to_string(GetCurrentDay());
}

bool YearIsValid()
{
    try
    {
        const auto year = std::stoi(YEAR);
        return year >= 2015 && year <= GetCurrentYear();
    }
    catch(...)
    {
    }

    std::cerr << "Error: '" << YEAR << "' must be between 2015 and current year inclusive\n";

    return false;
}

bool TryCreateSolutionDirectory(const std::string& rootPath)
{
    if(std::filesystem::exists(rootPath) && !FORCE_OVERWRITE)
    {
        std::cout << "Solution directory already exists\n";
        return false;
    }

    std::filesystem::create_directory(rootPath);
    return std::filesystem::exists(rootPath);
}

bool CreateCMakeLists(const std::filesystem::path& x)
{
    const auto cmakeLists = x / "CMakeLists.txt";
    if(std::ofstream ofs{cmakeLists}; ofs.is_open())
    {
        ofs << "# File auto-generated by StartNewDay\n";
        ofs << "cmake_minimum_required(VERSION 3.0...3.5)\n";
        ofs << "project(day" << DAY << " CXX)\n";
        ofs << "\n";

        const auto solutionFile = DAY + ".cpp";
        ofs << "add_executable(${PROJECT_NAME} " << solutionFile << ")\n";
        ofs << "target_link_libraries(${PROJECT_NAME} GTest::gtest_main AOC)\n";
        ofs << "\n";

        ofs << "include(GoogleTest)\n";
        ofs << "gtest_discover_tests(${PROJECT_NAME})\n";

#ifdef WIN32
        ofs << "\n";

        ofs << "if(WIN32)\n";
        ofs << "\tset_target_properties(${PROJECT_NAME} PROPERTIES FOLDER \"Solutions\")"
            << "\n";
        ofs << "target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/inputs/" + DAY +
                   ".txt ${CMAKE_SOURCE_DIR}/inputs/" + DAY + "_sample.txt)\n";
        ofs << "endif()\n";
#endif

        CREATED_FILES.push_back(cmakeLists.generic_string());

        return true;
    }

    return false;
}

// TODO: Use an in-file for this?
bool CreateSourceFiles(const std::filesystem::path& x)
{
    const auto solutionFile = DAY + ".cpp";
    const auto fullPath = x / solutionFile;

    if(std::ofstream ofs{fullPath}; ofs.is_open())
    {
        ofs << "// File auto-generated by StartNewDay\n";
        ofs << "\n";

        ofs << "#include <ACSolver.ipp>\n";
        ofs << "#include <Utilities.ipp>\n";
        ofs << "#include <gtest/gtest.h>\n";
        ofs << "\n";

        ofs << "namespace\n";
        ofs << "{\n";
        ofs << "    struct Solver : public ACSolver\n";
        ofs << "    {\n";
        ofs << "        Answer solvePartOne() override\n";
        ofs << "        {\n";
        ofs << "            return ACSolver::solvePartOne();\n";
        ofs << "        }\n";
        ofs << "\n";
        ofs << "        Answer solvePartTwo() override\n";
        ofs << "        {\n";
        ofs << "            return ACSolver::solvePartTwo();\n";
        ofs << "        }\n";
        ofs << "    };\n";
        ofs << "\n";
        ofs << "    Solver CreateSolver(bool useSample)\n";
        ofs << "    {\n";
        ofs << "        const auto inputFile = useSample ? \"" << DAY << "_sample.txt\" : \"" << DAY
            << ".txt\";\n";
        ofs << "        Solver solver;\n";
        ofs << "        solver.mInput = util::Parse(util::GetInputFile(inputFile));\n";
        ofs << "        return solver;\n";
        ofs << "    }\n";
        ofs << "}\n";
        ofs << "\n";
        ofs << "TEST(day_" << DAY << ", part_1)\n";
        ofs << "{\n";
        ofs << "    const auto answer = CreateSolver(false).solvePartOne();\n";
        ofs << "    ASSERT_NE(answer, std::nullopt);\n";
        ofs << "    // EXPECT_EQ(*answer, INSERT_CORRECT_ANSWER);\n";
        ofs << "    std::cout << \"[::::::::::] \" << *answer << std::endl;\n";
        ofs << "}\n";
        ofs << "\n";
        ofs << "TEST(day_" << DAY << ", part_2)\n";
        ofs << "{\n";
        ofs << "    const auto answer = CreateSolver(false).solvePartTwo();\n";
        ofs << "    ASSERT_NE(answer, std::nullopt);\n";
        ofs << "    // EXPECT_EQ(*answer, INSERT_CORRECT_ANSWER);\n";
        ofs << "    std::cout << \"[::::::::::] \" << *answer << std::endl;\n";
        ofs << "}\n";
        ofs << "\n";
        ofs << "TEST(day_" << DAY << ", sample_part_1)\n";
        ofs << "{\n";
        ofs << "    const auto answer = CreateSolver(true).solvePartOne();\n";
        ofs << "    ASSERT_NE(answer, std::nullopt);\n";
        ofs << "    // EXPECT_EQ(*answer, PART_ONE_SAMPLE_ANSWER);\n";
        ofs << "    std::cout << \"[::::::::::] \" << *answer << std::endl;\n";
        ofs << "}\n";
        ofs << "\n";
        ofs << "TEST(day_" << DAY << ", sample_part_2)\n";
        ofs << "{\n";
        ofs << "    const auto answer = CreateSolver(true).solvePartTwo();\n";
        ofs << "    ASSERT_NE(answer, std::nullopt);\n";
        ofs << "    // EXPECT_EQ(*answer, PART_TWO_SAMPLE_ANSWER);\n";
        ofs << "    std::cout << \"[::::::::::] \" << *answer << std::endl;\n";
        ofs << "}\n";

        CREATED_FILES.push_back(fullPath.generic_string());

        return true;
    }

    return false;
}

// TODO: Make sure file doesn't already exist.
bool DownloadInput()
{
    HttpsRequest request;
    request.setUrl("https://adventofcode.com/" + YEAR + "/day/" + DAY + "/input");
    request.setContentType("text/plain");
    if(const auto content = request())
    {
        const auto fileName = config::GetInputFilePath() + "/" + DAY + ".txt";
        if(std::ofstream ofs{config::GetInputFilePath() + "/" + DAY + ".txt"}; ofs.is_open())
        {
            ofs << *content;

            CREATED_FILES.push_back(fileName);

            return true;
        }
    }

    return false;
}

// TODO: Make sure file doesn't already exist.
bool DownloadSampleInput()
{
    HttpsRequest request;
    request.setUrl("https://adventofcode.com/" + YEAR + "/day/" + DAY);
    request.setContentType("text/html");
    if(const auto content = request())
    {
        // Beginning of sample input starts with "<pre><code>" and ends with "</code></pre>"
        // Ex:
        // <pre><code>A Y
        // B X
        // C Z
        // </code></pre>
        const std::string startTags = "<pre><code>";
        const auto beginPos = (*content).find(startTags) + startTags.size();
        const auto endTags = "</code></pre>";
        const auto size = (*content).find(endTags) - beginPos;

        const auto fileName = config::GetInputFilePath() + "/" + DAY + "_sample.txt";
        if(std::ofstream ofs{fileName}; ofs.is_open())
        {
            ofs << (*content).substr(beginPos, size);

            CREATED_FILES.push_back(fileName);

            return true;
        }
    }

    return false;
}

int main(int argc, char** argv)
{
    cxxopts::Options options{
        "StartNewDay", "Create C++ stub code for new Advent of Code challenge and download input"};

    // TODO: Positional arguments are not showing up in help
    // clang-format off
	options.add_options()
		("day", "The day number to use", cxxopts::value<std::string>()->default_value(GetCurrentDayString()))
		("year", "The year to use", cxxopts::value<std::string>()->default_value(GetCurrentYearString()))
		("f,force", "Force overwrite", cxxopts::value<bool>()->default_value("false"))
		("h,help", "Shows this help message")
		;
    // clang-format on

    options.parse_positional({"day", "year"});

    try
    {
        auto result = options.parse(argc, argv);

        if(result.count("help"))
        {
            std::cout << options.help() << "\n";
            return EXIT_SUCCESS;
        }

        DAY = result["day"].as<std::string>();
        if(!DayIsValid())
        {
            return EXIT_FAILURE;
        }

        YEAR = result["year"].as<std::string>();
        if(!YearIsValid())
        {
            return EXIT_FAILURE;
        }

        FORCE_OVERWRITE = result["force"].as<bool>();

        std::filesystem::path solutionsPath{config::GetSolutionsPath()};
        if(!std::filesystem::exists(solutionsPath))
        {
            std::cerr << "Could not find solutions path " << solutionsPath << "\n";
            return EXIT_FAILURE;
        }

        const auto newDayPath = solutionsPath / DAY;
        if(!TryCreateSolutionDirectory(newDayPath.generic_string()))
        {
            std::cerr << "Could not create new subdirectory " << newDayPath << "\n";
            std::cout << options.help() << "\n";
            std::cerr << "\n";

            return EXIT_FAILURE;
        }

        if(FORCE_OVERWRITE)
        {
            std::cout << "Overwriting day " << DAY << "...\n";
        }
        else
        {
            std::cout << "Starting new day " << DAY << "...\n";
        }

        if(!CreateCMakeLists(newDayPath))
        {
            std::cerr << "Could not create CMakeLists.txt for " << newDayPath << "\n";
            return EXIT_FAILURE;
        }

        if(!CreateSourceFiles(newDayPath))
        {
            std::cerr << "Could not create source files for " << newDayPath << "\n";
            return EXIT_FAILURE;
        }

        if(!DownloadInput())
        {
            std::cerr << "Could not download input\n";
            return EXIT_FAILURE;
        }

        if(!DownloadSampleInput())
        {
            // This is not a deal breaker. Just grab it yourself ya bum.
            std::cerr << "Could not download sample input\n";
        }

        std::cout << "Files created:\n";
        for(const auto& file : CREATED_FILES)
        {
            std::cout << "\t" << file << "\n";
        }

        std::cout << "\n";
        std::cout << "Re-run CMake to build the new day\n";

        return EXIT_SUCCESS;
    }
    catch(const cxxopts::exceptions::parsing& e)
    {
        std::cerr << "Error parsing option: " << e.what() << '\n';
        std::cout << options.help() << "\n";
    }
    catch(const cxxopts::exceptions::option_has_no_value& e)
    {
        std::cerr << "Error parsing required option: " << e.what() << '\n';
        std::cout << options.help() << "\n";
    }
    catch(const cxxopts::exceptions::specification& e)
    {
        std::cerr << "Error defining option specification: " << e.what() << '\n';
        std::cerr << "Please report this as a bug: "
                     "https://github.com/jonathondgebhardt/advent-of-code-cpp/issues\n";
    }
    catch(const cxxopts::exceptions::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        std::cout << options.help() << "\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
    catch(...)
    {
        std::cerr << "Unknown error\n";
    }

    return EXIT_FAILURE;
}
