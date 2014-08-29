///////////////////////////////////////////////////////////////////////////////
/// \file adjacent_difference.hpp
///   Contains range-based versions of the std generic numeric operation:
///     adjacent_difference.
//
// Copyright 2004 Eric Niebler.
// Copyright 2014 Gonzalo Brito Gadeschi.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Implementation based on the code in libc++
//   http://http://libcxx.llvm.org/

#ifndef RANGES_V3_NUMERIC_ADJACENT_DIFFERENCE_HPP
#define RANGES_V3_NUMERIC_ADJACENT_DIFFERENCE_HPP

#include <range/v3/begin_end.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/utility/invokable.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/range_algorithm.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/iterator_concepts.hpp>

namespace ranges
{
    inline namespace v3
    {
        template <typename I, typename O, typename BOp = minus, typename P = ident,
                  typename V = iterator_value_t<I>,
                  typename X = concepts::Invokable::result_t<P, V>,
                  typename Y = concepts::Invokable::result_t<BOp, X, X>>
        constexpr bool AdjacentDifferentiable()
        {
            return InputIterator<I>() && WeakOutputIterator<O, Y>() &&
                   Invokable<P, V>() && Invokable<BOp, X, X>();
        }

        struct adjacent_difference_fn
        {
            template <typename I, typename S, typename O, typename BOp = minus,
                      typename P = ident,
                      CONCEPT_REQUIRES_(Sentinel<S, I>() &&
                                        AdjacentDifferentiable<I, O, BOp, P>())>
            auto operator()(I begin, S end, O result, BOp bop_ = BOp{},
                            P proj_ = P{}) const -> std::pair<I, O>
            {
                auto &&bop = invokable(bop_);
                auto &&proj = invokable(proj_);

                if(begin != end)
                {
                    auto t1(proj(*begin));
                    *result = t1;
                    for(++begin, ++result; begin != end; ++begin, ++result)
                    {
                        auto t2(proj(*begin));
                        *result = bop(t2, t1);
                        t1 = std::move(t2);
                    }
                }
                return std::pair<I, O>{begin, result};
            }

            template <typename I, typename S, typename O, typename S2,
                      typename BOp = minus, typename P = ident,
                      CONCEPT_REQUIRES_(Sentinel<S, I>() && Sentinel<S2, O>() &&
                                        AdjacentDifferentiable<I, O, BOp, P>())>
            auto operator()(I begin, S end, O result, S2 end_result, BOp bop_ = BOp{},
                            P proj_ = P{}) const -> std::pair<I, O>
            {
                auto &&bop = invokable(bop_);
                auto &&proj = invokable(proj_);

                if(begin != end && result != end_result)
                {
                    auto t1(proj(*begin));
                    *result = t1;
                    for(++begin, ++result; begin != end && result != end_result;
                        ++begin, ++result)
                    {
                        auto t2(proj(*begin));
                        *result = bop(t2, t1);
                        t1 = std::move(t2);
                    }
                }
                return std::pair<I, O>{begin, result};
            }

            template <typename Rng, typename O, typename BOp = minus, typename P = ident,
                      typename I = range_iterator_t<Rng>,
                      typename O2 = detail::uncvref_t<O>,
                      CONCEPT_REQUIRES_(Iterable<Rng>() &&
                                        AdjacentDifferentiable<I, O2, BOp, P>())>
            auto operator()(Rng &rng, O &&result, BOp bop = BOp{},
                            P proj = P{}) const -> std::pair<I, O>
            {
                return (*this)(begin(rng), end(rng), result, std::move(bop),
                               std::move(proj));
            }

            template <typename Rng, typename ORng, typename BOp = minus,
                      typename P = ident, typename I = range_iterator_t<Rng>,
                      typename O = range_iterator_t<ORng>,
                      CONCEPT_REQUIRES_(Iterable<Rng>() && Iterable<ORng>() &&
                                        AdjacentDifferentiable<I, O, BOp, P>())>
            auto operator()(Rng &rng, ORng &result, BOp bop = BOp{},
                            P proj = P{}) const -> std::pair<I, O>
            {
                return (*this)(begin(rng), end(rng), begin(result), end(result),
                               std::move(bop), std::move(proj));
            }
        };

        RANGES_CONSTEXPR adjacent_difference_fn adjacent_difference{};
    }
}

#endif
