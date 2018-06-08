#pragma once
#include "index_sequence.h"

namespace tinystl
{
    template <typename... Args>
    class tuple
    {
    };

    template <typename T, typename... Args>
    class tuple<T, Args...> : public tuple<Args...>
    {
        public:
            tuple() = default;

            tuple(T&& t, Args&&... args)
                : tuple<Args...>(std::forward<Args>(args)...),
                  value_(std::forward<T>(t))
            { }

            template <std::size_t N>
            decltype(auto) get() {
                static_assert(N<=sizeof...(Args));
                if constexpr (N == 0) {
                    return (value_);
                }
                else {
                    return (this->tuple<Args...>::template get<N - 1>());
                }
            }

            template <std::size_t N>
            decltype(auto) get() const {
                static_assert(N<=sizeof...(Args));
                if constexpr (N == 0) {
                    return (value_);
                }
                else {
                    return (this->tuple<Args...>::template get<N - 1>());
                }
            }
        protected:
            T value_;
    };

    template <std::size_t N, typename TupleType>
    decltype(auto) get(TupleType&& t) {
        return (t.template get<N>());
    }


    template <std::size_t N, typename TupleType>
    struct tuple_element
    {
        using type = void;
    };

    template <std::size_t N, template <typename...> class Tuple, typename... ArgsWrapper>
    struct tuple_element<N, Tuple<ArgsWrapper...>>
    {
        template <std::size_t M, typename T, typename... Args>
        struct tuple_element_helper : public tuple_element_helper<M-1, Args..., void>
        {
        };

        template <typename T, typename... Args>
        struct tuple_element_helper<0, T, Args...>
        {
            using type = T;
        };

        using type = typename tuple_element_helper<N, ArgsWrapper..., void>::type;
    };

    template <std::size_t N, typename Tuple>
    using tuple_element_t = typename tuple_element<N, Tuple>::type;

    template <typename Tuple>
    struct tuple_size : public std::integral_constant<std::size_t, 0>
    {
    };

    template <template <typename...> class Tuple, typename... Args>
    struct tuple_size<Tuple<Args...>> : public std::integral_constant<std::size_t, sizeof...(Args)>
    {
    };

    template <typename Tuple>
    static constexpr inline auto tuple_size_v = tuple_size<Tuple>::value;


    template <typename... Args>
    tuple<Args...> make_tuple(Args&&... args) {
        return tuple<Args...>(std::forward<Args>(args)...);
    }

    namespace detail
    {
        template <std::size_t N1, std::size_t M1, std::size_t N2, std::size_t M2, typename Tuple1, typename Tuple2, typename... Args>
        auto tuple_concat_impl(Tuple1&& t1, Tuple2&& t2, Args&&... args) {
            if constexpr (N1 == M1 && N2 == M2) {
                return make_tuple(std::forward<Args>(args)...);
            }
            else if constexpr (N1 == M1) {
                return tuple_concat_impl<N1, M1, N2+1, M2>(std::forward<Tuple1>(t1),
                                                           std::forward<Tuple2>(t2),
                                                           std::forward<Args>(args)...,
                                                           t2.template get<N2>());
            }
            else {
                return tuple_concat_impl<N1+1, M1, N2, M2>(std::forward<Tuple1>(t1),
                                                           std::forward<Tuple2>(t2),
                                                           std::forward<Args>(args)...,
                                                           t1.template get<N1>());
            }
        }
        template <typename Tuple, typename Func, std::size_t... Idx>
        void for_each_impl(Tuple&& t, Func&& f, tinystl::index_sequence<Idx...>) {
            (std::forward<Func>(f)(get<Idx>(std::forward<Tuple>(t))), ...);
        }
    }
    template <typename... Args1, typename... Args2>
    auto tuple_concat(const tuple<Args1...>& t1, const tuple<Args2...>& t2) {
        return detail::tuple_concat_impl<0, sizeof...(Args1), 0, sizeof...(Args2)>(t1, t2);
    }


    template <typename Tuple, typename Func>
    void for_each(Tuple&& t, Func&& f) {
        detail::for_each_impl(std::forward<Tuple>(t),
                              std::forward<Func>(f),
                              tinystl::make_index_sequence<tuple_size_v<std::remove_reference_t<Tuple>>>{});
    }
}
