// Range v3 library
//
//  Copyright Eric Niebler 2014
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <range/v3/algorithm/min.hpp>
#include <cassert>
#include <memory>
#include <random>
#include <numeric>
#include <algorithm>
#include "../simple_test.hpp"
#include "../test_utils.hpp"
#include "../test_iterators.hpp"

namespace { std::mt19937 gen; }

template <class Iter, class Sent = Iter>
void
test_iter(Iter first, Sent last)
{
    assert(first != last);
    auto rng = ranges::make_iterator_range(first, last);
    auto v1 = ranges::min(rng);
    for (Iter i = first; i != last; ++i)
        CHECK(!(*i < v1));
}

template <class Iter, class Sent = Iter>
void
test_iter(unsigned N)
{
    assert(N > 0);
    std::unique_ptr<int[]> a{new int[N]};
    std::iota(a.get(), a.get()+N, 0);
    std::shuffle(a.get(), a.get()+N, gen);
    test_iter(Iter(a.get()), Sent(a.get()+N));
}

template <class Iter, class Sent = Iter>
void
test_iter()
{
    test_iter<Iter, Sent>(1);
    test_iter<Iter, Sent>(2);
    test_iter<Iter, Sent>(3);
    test_iter<Iter, Sent>(10);
    test_iter<Iter, Sent>(1000);
}

template <class Iter, class Sent = Iter>
void
test_iter_comp(Iter first, Sent last)
{
    assert(first != last);
    auto rng = ranges::make_iterator_range(first, last);
    auto v = ranges::min(rng, std::greater<int>());
    for (Iter i = first; i != last; ++i)
        CHECK(!std::greater<int>()(*i, v));
}

template <class Iter, class Sent = Iter>
void
test_iter_comp(unsigned N)
{
    assert(N > 0);
    std::unique_ptr<int[]> a{new int[N]};
    std::iota(a.get(), a.get()+N, 0);
    std::shuffle(a.get(), a.get()+N, gen);
    test_iter_comp(Iter(a.get()), Sent(a.get()+N));
}

template <class Iter, class Sent = Iter>
void
test_iter_comp()
{
    test_iter_comp<Iter, Sent>(1);
    test_iter_comp<Iter, Sent>(2);
    test_iter_comp<Iter, Sent>(3);
    test_iter_comp<Iter, Sent>(10);
    test_iter_comp<Iter, Sent>(1000);
}

struct S
{
    int i;
};

int main()
{
    test_iter<input_iterator<const int*> >();
    test_iter<forward_iterator<const int*> >();
    test_iter<bidirectional_iterator<const int*> >();
    test_iter<random_access_iterator<const int*> >();
    test_iter<const int*>();
    test_iter<input_iterator<const int*>, sentinel<const int*>>();
    test_iter<forward_iterator<const int*>, sentinel<const int*>>();
    test_iter<bidirectional_iterator<const int*>, sentinel<const int*>>();
    test_iter<random_access_iterator<const int*>, sentinel<const int*>>();

    test_iter_comp<input_iterator<const int*> >();
    test_iter_comp<forward_iterator<const int*> >();
    test_iter_comp<bidirectional_iterator<const int*> >();
    test_iter_comp<random_access_iterator<const int*> >();
    test_iter_comp<const int*>();
    test_iter_comp<input_iterator<const int*>, sentinel<const int*>>();
    test_iter_comp<forward_iterator<const int*>, sentinel<const int*>>();
    test_iter_comp<bidirectional_iterator<const int*>, sentinel<const int*>>();
    test_iter_comp<random_access_iterator<const int*>, sentinel<const int*>>();

    // Works with projections?
    S s[] = {S{1},S{2},S{3},S{4},S{-4},S{5},S{6},S{7},S{8},S{9}};
    S v = ranges::min(s, std::less<int>{}, &S::i);
    CHECK(v.i == -4);

    return test_result();
}