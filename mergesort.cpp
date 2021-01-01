// 12/31/2020

#include <iostream>

#include "tmplist.hpp"

// In this code, the first template parameter to some templates is T. This allows
// the lists we use to be generic. T can be any type comparable with <= that
// can also be passed in as a non-type template parameter.
// This code works with C++11 or above.



// T, L1, L2 should be same as passed to Merge. bool should be L1::head <= L2::head.
// This has 3 outputs: val (the min of the 2 heads), L1new, and L2new (the two
// lists, with either L1's head or L2's head removed, depending on which is the min).
template <class L1, class L2, bool> // this is the true case
struct MergeHelper {
    enum {val = L1::head};
    using L1new = typename L1::Tail;
    using L2new = L2;
};

template <class L1, class L2>
struct MergeHelper<L1, L2, false> {
    enum {val = L2::head};
    using L1new = L1;
    using L2new = typename L2::Tail;
};


// First is the Merge routine: given two Lists, merge them. This computes the
// output list as the ::Type member.
// L1 and L2 are the input lists. They can be List or EmptyList types.
template <class T, class L1, class L2>  // Recursive case
struct Merge {
  private: // Note: comparing enums is a warning, -Wdeprecated-anon-enum-enum-conversion. See: https://stackoverflow.com/a/15772997
    using Helper = MergeHelper<L1, L2, L1::head <= L2::head>;
  public:
    using Type = tmplist::List<T, Helper::val, typename Merge<T, typename Helper::L1new, typename Helper::L2new>::Type>;
};

template <class T, class L1>  // Base case 1
struct Merge<T, L1, tmplist::EmptyList<T>> {
    using Type = L1;
};

template <class T, class L2>  // Base case 2
struct Merge<T, tmplist::EmptyList<T>, L2> {
    using Type = L2;
};



// Here is the template which should be used by the end user: Sort.
// The input is a list L which may be a List or an EmptyList. The output is a
// sorted List (or EmptyList) type, in the ::Type member.
template <class T, class L>  // Forward declaration
struct Sort;


// Helper template which uses len (the length of the list) to specialize
// for the base cases.
template <class T, class L, std::size_t len>  // Recursive case
struct SortImpl {
    // This is a classic top-down mergesort: split in half, sort each sublist,
    // and merge the two back together.
  private:
    static_assert(len == tmplist::Length<T, L>::val, "Incorrect len in SortImpl recursive case.");
    enum {split_idx = len / 2};
    using SplitResult = tmplist::Split<T, split_idx, L>;
  public:
    using Type = typename Merge<T, 
        typename Sort<T, typename SplitResult::Left>::Type, 
        typename Sort<T, typename SplitResult::Right>::Type>::Type;
};

template <class T, class L>  // Base case for length 1
struct SortImpl<T, L, 1> {
    static_assert(tmplist::Length<T, L>::val == 1, "Incorrect len in SortImpl length-1 case.");
    using Type = L;
};

template <class T>  // Base case for length 0
struct SortImpl<T, tmplist::EmptyList<T>, 0> {
    using Type = tmplist::EmptyList<T>;
};


template <class T, class L>  // Definition
struct Sort {
    using Type = typename SortImpl<T, L, tmplist::Length<T, L>::val>::Type;
};



int main() {
    using Empty = tmplist::ListConstructor<int>::Type;
    using EmptySorted = Sort<int, Empty>::Type;
    using List1 = tmplist::ListConstructor<unsigned int, 0>::Type;
    using List1Sorted = Sort<unsigned int, List1>::Type;
    using List2 = tmplist::ListConstructor<int, 1, 2, 3>::Type;
    using List2Sorted = Sort<int, List2>::Type;
    using List3 = tmplist::ListConstructor<int, 3, -1, 2>::Type;
    using List3Sorted = Sort<int, List3>::Type;
    using List4 = tmplist::ListConstructor<int, -245, 1025, 3, 0, -2348>::Type;
    using List4Sorted = Sort<int, List4>::Type;

    std::cout << "should be 0: " << tmplist::Length<int, EmptySorted>::val << std::endl;

    std::cout << "should be 1: " << tmplist::Length<unsigned int, List1Sorted>::val << std::endl;
    std::cout << "should be 0: " << tmplist::Get<unsigned int, 0, List1Sorted>::val << std::endl;

    std::cout << "should be 3: " << tmplist::Length<int, List2Sorted>::val << std::endl;
    std::cout << "should be 1: " << tmplist::Get<int, 0, List2Sorted>::val << std::endl;
    std::cout << "should be 2: " << tmplist::Get<int, 1, List2Sorted>::val << std::endl;
    std::cout << "should be 3: " << tmplist::Get<int, 2, List2Sorted>::val << std::endl;

    std::cout << "should be 3: " << tmplist::Length<int, List3Sorted>::val << std::endl;
    std::cout << "should be -1: " << tmplist::Get<int, 0, List3Sorted>::val << std::endl;
    std::cout << "should be 2: " << tmplist::Get<int, 1, List3Sorted>::val << std::endl;
    std::cout << "should be 3: " << tmplist::Get<int, 2, List3Sorted>::val << std::endl;

    std::cout << "should be 5: " << tmplist::Length<int, List4Sorted>::val << std::endl;
    std::cout << "should be -2348: " << tmplist::Get<int, 0, List4Sorted>::val << std::endl;
    std::cout << "should be -245: " << tmplist::Get<int, 1, List4Sorted>::val << std::endl;
    std::cout << "should be 0: " << tmplist::Get<int, 2, List4Sorted>::val << std::endl;
    std::cout << "should be 3: " << tmplist::Get<int, 3, List4Sorted>::val << std::endl;
    std::cout << "should be 1025: " << tmplist::Get<int, 4, List4Sorted>::val << std::endl;
    
    // Should cause compile errors
    //std::cout << tmplist::Get<int, 5, List4Sorted>::val << std::endl;
    //std::cout << tmplist::Get<int, 1, EmptySorted>::val << std::endl;
    return 0;
}
