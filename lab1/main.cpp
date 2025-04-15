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
//   Information om komplettering:
//   Kompletteringen kan gälla hela filen och alla filer i labben,
//   så får ni komplettering på en sak, kan samma sak förekomma på
//   fler ställen utan att jag skrivit det.
//
//   Komplettering lämnas in via sendlab inom 5 arbetsdagar.
//   Har ni frågor om kompletteringen kan ni maila mig.

// Komplettering: Fånga exceptions på lämpligt sätt. Användaren ska aldrig behöva se “Terminate called after...”.
// Komplettering: Spara inte undan flags och params i egna vectorer i onödan
// Komplettering: Remove fungerar ej enligt specifikation i lab-pm
// Komplettering: Använd const& för att undvika onödig kopiering (se även parametrar till lambda och range-baserade
// loopar) Komplettering: Upprepning vid utskrift av table och frequency
//          Tips: tänk på att left och right är sticky

void op_print(const vector<string>& text)
{
    std::copy(text.begin(), text.end(), std::ostream_iterator<std::string>{std::cout, " "});
    std::cout << std::endl;
}

vector<pair<string, size_t>> get_frequency(const vector<string>& text)
{
    std::set<string> unique{text.begin(), text.end()};

    vector<pair<string, size_t>> frequencies{};
    std::transform(unique.begin(), unique.end(), std::back_inserter(frequencies), [&text](const string& word) {
        size_t occurences = std::count(text.begin(), text.end(), word);
        return std::make_pair(word, occurences);
    });

    return frequencies;
}

void op_frequency(const vector<string>& text, bool table)
{
    vector<pair<string, size_t>> frequencies{get_frequency(text)};

    auto sort_lambda = [table](const auto& a, const auto& b) {
        if (table)
            return a.first < b.first;
        else
            return a.second > b.second;
    };

    std::sort(frequencies.begin(), frequencies.end(), sort_lambda);

    auto max_it = std::max_element(frequencies.begin(), frequencies.end(),
                                   [](const auto& a, const auto& b) { return a.first.size() < b.first.size(); });
    size_t max_len{max_it->first.size()};

    if (table)
        cout << std::left;
    else
        cout << std::right;

    for (const auto& elem : frequencies)
    {
        cout << std::setw(max_len) << elem.first << " " << elem.second << endl;
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
    auto it = std::remove(text.begin(), text.end(), parameter);
    text.erase(it, text.end());
}

void dispatch(vector<string>& text, const string& flag, const string& parameter)
{
    if (flag == "print")
    {
        op_print(text);
    }
    else if (flag == "frequency")
    {
        op_frequency(text, false);
    }
    else if (flag == "table")
    {
        op_frequency(text, true);
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
        throw std::runtime_error("Invalid flag: \"" + flag + "\"");
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::string usage{"Usage: "};
        std::string prog{argv[0]};
        std::string file{" [file]"};
        std::cout << "Usage: " << argv[0] << " [file]" << std::endl;
        return 1;
    }

    std::ifstream file{argv[1]};

    if (!file.is_open())
    {
        std::cout << "File " << argv[1] << " cannot be opened!" << std::endl;
    }

    std::vector<std::string> arguments{&argv[2], &argv[argc]};

    std::vector<std::string> text{};
    std::copy(std::istream_iterator<std::string>{file}, std::istream_iterator<std::string>{}, std::back_inserter(text));

    for (const std::string& arg : arguments)
    {
        size_t mid = arg.find("=");
        try
        {
            if (mid == arg.npos)
                dispatch(text, arg.substr(2, arg.size()), "");
            else
                dispatch(text, arg.substr(2, mid - 2), arg.substr(mid + 1, arg.size()));
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    return 0;
}