#ifndef TMPLIST_H
#define TMPLIST_H

// This is a small, single-header list library for template metaprogramming.
// This is a fully compile-time list data structure. Each unique list is a
// unique instantiation of the List template, which is essentially a cons
// cell. The List template contains a head member, which is the current value
// in the list, and a Tail member which is the type corresponding to the rest
// of the list.
// To work with the list, you recursively unpack it, just as you would in
// functional programming. The base case is the EmptyList type.
//
// The first template parameter for all templates is a type T. These list
// templates are generic, and this template parameter is the type of the lists.
//
// Rule of thumb: values have lowercase names. Types have capitalized names.
//
// This library was heavily inspired by another TMP list library on the Code
// Review stack: https://codereview.stackexchange.com/q/172566
//
// This library works with C++11 or above.


#include <cstddef> // size_t

namespace tmplist {
    
    // This is our sentinel type.
    template <class T>
    struct EmptyList {};

    // This is the underlying implementation of the list template. This
    // should (usually) not be used directly by the end user. Instead use the
    // ListConstructor template.
    // The one reason for an end-user to use this directly is as a prepending
    // helper routine: this can be used to prepend a value to a list.
    //
    // The type Rest should be a List or EmptyList.
    template <class T, T first, class Rest>
    struct List {
        enum {head = first};
        using Tail = Rest;
    };


    // The output of ListConstructor is the Type member. It will be
    // either a List or EmptyList.

    // This is the base case. Should be true iff vals is empty.
    template <class T, T... vals>
    struct ListConstructor {
        static_assert(sizeof...(vals) == 0, "ListConstructor base case has a non-empty list.");

        using Type = EmptyList<T>;
    };

    // Recursive case. rest may be empty or non-empty.
    template <class T, T first, T... rest>
    struct ListConstructor<T, first, rest...> {
        using Type = List<T, first, typename ListConstructor<T, rest...>::Type>;
    };


    // Below are various utility routines which act on Lists.

    // Computes the length of the list. The output is stored in the ::val member.
    // L is the list, should either be a List or EmptyList type.
    template <class T, class L>  // Recursive case
    struct Length {
        enum {val = 1 + Length<T, typename L::Tail>::val};
    };

    template <class T>  // Base case
    struct Length<T, EmptyList<T>> {
        enum {val = 0};
    };


    // Split: splits a list into 2 parts. The two output lists are stored in
    // the ::Left and ::Right members, either List or EmptyList.
    // i is the index where to split. The list will be split into [0,i) and [i,n).
    // L is the list. It should be either List or EmptyList.
    template <class T, std::size_t i, class L>  // Recursive case
    struct Split {
      private:
        using SplitTail = Split<T, i-1, typename L::Tail>;
      public:
        using Left = List<T, L::head, typename SplitTail::Left>;
        using Right = typename SplitTail::Right;
    };

    template <class T, class L>  // Base case
    struct Split<T, 0, L> {
        using Left = EmptyList<T>;
        using Right = L;
    };


    // Get: access the i-th element of the list. It will be stored in the ::val member.
    // i is the index to access (zero-indexed), L is the list. Must be a List, not
    // an EmptyList.
    template <class T, std::size_t i, class L>  // Recursive case
    struct Get {
        enum {val = Get<T, i-1, typename L::Tail>::val};
    };

    template <class T, class L>  // Base case
    struct Get<T, 0, L> {
        enum {val = L::head};
    };

}

#endif // TMPLIST_H
