#include <algorithm>
#include <iostream>
#include <string>

#include "list.h"

int main()
{
    // List<int> list{10, 4, 6, 7, 4, 2};
    List<std::string> list{"test1", "test3", "test2"};

    for (auto elem : list)
    {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;

    auto rb{std::make_reverse_iterator(list.end())};
    auto re{std::make_reverse_iterator(list.begin())};
    for (auto it = rb; it != re; ++it)
    {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;

    std::transform(list.begin(), list.end(), list.begin(), [](std::string val) { return val += "t"; });

    std::copy(list.begin(), list.end(), std::ostream_iterator<std::string>{std::cout, " "});
    std::cout << std::endl;

    return 0;
}