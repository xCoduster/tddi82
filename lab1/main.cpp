#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

void op_print(const vector<string>& text)
{
    std::copy(text.begin(), text.end(), std::ostream_iterator<std::string>{std::cout, " "});
    std::cout << std::endl;
}

void op_frequency(const vector<string>& text)
{
    // std::map<std::string, int> occurrenses;
    vector<string> unique{};
    std::copy(text.begin(), text.end(), std::back_inserter(unique));
    std::sort(unique.begin(), unique.end());
    auto unique_it = std::unique(unique.begin(), unique.end());
    unique.erase(unique_it, unique.end());

    vector<tuple<string, size_t>> frequencies{};
    std::transform(unique.begin(), unique.end(), std::back_inserter(frequencies), [&text](string word) {
        size_t occurences = std::count(text.begin(), text.end(), word);
        return std::make_tuple(word, occurences);
    });

    std::sort(frequencies.begin(), frequencies.end(), [](auto a, auto b) { return std::get<1>(a) > std::get<1>(b); });

    auto it = std::max_element(frequencies.begin(), frequencies.end(),
                               [](auto a, auto b) { return std::get<0>(a).size() > std::get<0>(b).size(); });
    size_t max_len = std::get<0>(*it).size();

    for (auto elem : frequencies)
    {
        cout << std::setw(max_len) << std::get<0>(elem) << " " << std::get<1>(elem) << endl;
    }
}

void op_table(const vector<string>& text)
{
}

void op_substitute(vector<string>& text)
{
}

void dispatch(vector<string>& text, const string& step, const string& data)
{
    if (step == "print")
    {
        op_print(text);
    }
    else if (step == "frequency")
    {
        op_frequency(text);
    }
    else if (step == "table")
    {
    }
    else if (step == "substitute")
    {
    }
    else if (step == "remove")
    {
    }
}

int main(int argc, char* argv[])
{
    std::ifstream file{argv[1]};

    std::vector<std::string> arguments{};
    std::transform(&argv[2], &argv[argc], std::back_inserter(arguments), [](char* s) { return std::string{s}; });

    std::vector<std::string> steps{};
    std::vector<std::string> additional_data{};

    std::for_each(arguments.begin(), arguments.end(), [&steps, &additional_data](std::string arg) {
        size_t mid = arg.find("=");

        if (mid == arg.npos)
        {
            arg.erase(0, 2);
            steps.push_back(arg);
            additional_data.push_back(std::string{});
        }
        else
        {
            steps.push_back(arg.substr(2, mid - 2));
            additional_data.push_back(arg.substr(mid + 1, arg.size()));
        }
    });

    std::vector<std::string> text{};
    std::copy(std::istream_iterator<std::string>{file}, std::istream_iterator<std::string>{}, std::back_inserter(text));

    for (size_t i{}; i < steps.size(); i++)
    {
        dispatch(text, steps[i], additional_data[i]);
    }

    return 0;
}