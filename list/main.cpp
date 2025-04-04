#include <algorithm>
#include <iostream>

#include "list.h"

int main()
{
    List list{10, 4, 6, 7, 4, 2};

    for (int elem : list)
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

    std::transform(list.begin(), list.end(), list.begin(), [](int val) { return val += 2; });

    std::copy(list.begin(), list.end(), std::ostream_iterator<int>{std::cout, " "});
    std::cout << std::endl;

    return 0;
}