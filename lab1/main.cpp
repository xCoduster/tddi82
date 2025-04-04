#include <algorithm>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

using namespace std;

void op_print(const vector<string>& text)
{
    std::copy(text.begin(), text.end(), std::ostream_iterator<std::string>{std::cout, " "});
    std::cout << std::endl;
}

vector<pair<string, size_t>> get_frequency(const vector<string>& text)
{
    std::set<string> unique{text.begin(), text.end()};

    vector<pair<string, size_t>> frequencies{};
    std::transform(unique.begin(), unique.end(), std::back_inserter(frequencies), [&text](string word) {
        size_t occurences = std::count(text.begin(), text.end(), word);
        return std::make_pair(word, occurences);
    });

    return frequencies;
}

size_t max(const vector<pair<string, size_t>>& container)
{
    auto it = std::max_element(container.begin(), container.end(),
                               [](auto a, auto b) { return a.first.size() < b.first.size(); });
    return it->first.size();
}

void op_frequency(const vector<string>& text)
{
    vector<pair<string, size_t>> frequencies{get_frequency(text)};
    std::sort(frequencies.begin(), frequencies.end(), [](auto a, auto b) { return a.second > b.second; });

    size_t max_len{max(frequencies)};

    for (auto elem : frequencies)
    {
        cout << std::setw(max_len) << elem.first << " " << elem.second << endl;
    }
}

void op_table(const vector<string>& text)
{
    vector<pair<string, size_t>> frequencies{get_frequency(text)};
    std::sort(frequencies.begin(), frequencies.end(), [](auto a, auto b) { return a.first < b.first; });

    size_t max_len{max(frequencies)};

    for (auto elem : frequencies)
    {
        cout << std::left << setw(max_len + 1) << elem.first;
        cout << std::right << elem.second << endl;
    }
}

void op_substitute(vector<string>& text, const string& parameter)
{
    std::string old_word{};
    std::string new_word{};

    size_t mid = parameter.find("+");

    if (mid == parameter.npos)
    {
        throw std::runtime_error("Invalid syntax");
    }
    else
    {
        old_word = parameter.substr(0, mid);
        new_word = parameter.substr(mid + 1, parameter.size());
    }

    std::replace(text.begin(), text.end(), old_word, new_word);
}

void op_remove(vector<string>& text, const string& parameter)
{
    std::remove(text.begin(), text.end(), parameter);
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
    else
    {
        throw std::runtime_error("Invalid flag");
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

    if (!file.is_open())
    {
        throw std::runtime_error("File cannot be read");
    }

    std::vector<std::string> arguments{&argv[2], &argv[argc]};

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