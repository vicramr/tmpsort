// Tests for tmplist.
#include <iostream>

#include "tmplist.hpp"

int main() {
    using Empty = tmplist::ListConstructor<int>::Type;
    using List1 = tmplist::ListConstructor<unsigned int, 0>::Type;
    using List2 = tmplist::ListConstructor<int, 1, 2, 3>::Type;


    std::cout << "should be 0: " << tmplist::Length<int, Empty>::val << std::endl;
    std::cout << "should be 1: " << tmplist::Length<unsigned int, List1>::val << std::endl;
    std::cout << "should be 1: " << tmplist::Length<int, tmplist::Split<int, 1, List2>::Left>::val << std::endl;
    std::cout << "should be 2: " << tmplist::Length<int, tmplist::Split<int, 1, List2>::Right>::val << std::endl;

    std::cout << "should be 0: " << tmplist::Get<unsigned int, 0, List1>::val << std::endl;
    std::cout << "should be 1: " << tmplist::Get<int, 0, List2>::val << std::endl;
    std::cout << "should be 2: " << tmplist::Get<int, 1, List2>::val << std::endl;
    std::cout << "should be 3: " << tmplist::Get<int, 2, List2>::val << std::endl;

    return 0;

    // The following should cause compile errors.
    
    // Error: accessing private member
    //std::cout << tmplist::Length<int, tmplist::Split<int, 1, List2>::SplitTail::Right>::val << std::endl;

    // Error: out of bounds
    //std::cout << tmplist::Get<int, 3, List2>::val << std::endl;
    //std::cout << tmplist::Get<unsigned int, 1, List1>::val << std::endl;
}
