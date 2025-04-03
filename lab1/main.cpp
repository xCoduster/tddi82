#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

void op_print(const vector<string>& text)
{
    std::copy(text.begin(), text.end(), std::ostream_iterator<std::string>{std::cout, " "});
    std::cout << std::endl;
}

void op_frequency(const vector<string>& text)
{
    vector<string> unique{};
    std::copy(text.begin(), text.end(), std::back_inserter(unique));
    std::sort(unique.begin(), unique.end());
    auto unique_it = std::unique(unique.begin(), unique.end());
    unique.erase(unique_it, unique.end());

    vector<pair<string, size_t>> frequencies{};
    std::transform(unique.begin(), unique.end(), std::back_inserter(frequencies), [&text](string word) {
        size_t occurences = std::count(text.begin(), text.end(), word);
        return std::make_pair(word, occurences);
    });

    std::sort(frequencies.begin(), frequencies.end(), [](auto a, auto b) { return a.second > b.second; });

    auto max_it = std::max_element(frequencies.begin(), frequencies.end(),
                                   [](auto a, auto b) { return a.first.size() < b.first.size(); });
    size_t max_len = max_it->first.size();

    for (auto elem : frequencies)
    {
        cout << std::setw(max_len) << elem.first << " " << elem.second << endl;
    }
}

void op_table(const vector<string>& text)
{
}

void op_substitute(vector<string>& text, const string& parameter)
{
}

void op_remove(vector<string>& text, const string& parameter)
{
}

void dispatch(vector<string>& text, const string& flag, const string& parameter)
{
    if (flag == "print")
    {
        op_print(text);
    }
    else if (flag == "frequency")
    {
        op_frequency(text);
    }
    else if (flag == "table")
    {
        op_table(text);
    }
    else if (flag == "substitute")
    {
        op_substitute(text, parameter);
    }
    else if (flag == "remove")
    {
        op_remove(text, parameter);
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::string usage{"Usage: "};
        std::string prog{argv[0]};
        std::string file{" [file]"};
        throw std::runtime_error(usage + prog + file);
    }

    std::ifstream file{argv[1]};

    std::vector<std::string> arguments{};
    std::transform(&argv[2], &argv[argc], std::back_inserter(arguments), [](char* s) { return std::string{s}; });

    std::vector<std::string> text{};
    std::copy(std::istream_iterator<std::string>{file}, std::istream_iterator<std::string>{}, std::back_inserter(text));

    std::vector<std::string> flags{};
    std::vector<std::string> parameters{};

    std::for_each(arguments.begin(), arguments.end(), [&flags, &parameters](std::string arg) {
        size_t mid = arg.find("=");

        if (mid == arg.npos)
        {
            arg.erase(0, 2);
            flags.push_back(arg);
            parameters.push_back(std::string{});
        }
        else
        {
            flags.push_back(arg.substr(2, mid - 2));
            parameters.push_back(arg.substr(mid + 1, arg.size()));
        }
    });

    for (size_t i{}; i < flags.size(); i++)
    {
        dispatch(text, flags[i], parameters[i]);
    }

    return 0;
}