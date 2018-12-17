/*

Copyright (c) 2018 MacroBull

*/

#pragma once

#include <algorithm> // std::reverse
#include <stack>

#include "raw.h"
#include "traits_stl.h"
#include "traits_stl_adaptor.h"

namespace NAMESPACE
{

/*
 * for:
 *      std::stack<TI, ...>
 *
 */
template <template <typename...> class  F, typename TB,
          typename TI, typename ...Args>
struct RawSerializationMultiplexer<F, TB, std::stack<TI, Args...>,
        enable_if_t<
            is_either_serializer<F>::value
        >>
{
    using TC = container_type_t<std::stack<TI, Args...>>;

    inline TB operator()(TB buffer, std::stack<TI, Args...>& adaptor) const
    {
        TC container;
        auto adaptor_(adaptor); // copy

        while (!adaptor_.empty())
        {
            container.push_back(std::move(adaptor_.top()));
            adaptor_.pop();
        }

        std::reverse(std::begin(container), std::end(container));
        return RawSerializationMultiplexer<F, TB, TC>()(buffer, container);
    }
};

template <template <typename...> class  F, typename TB,
          typename TI, typename ...Args>
struct RawSerializationMultiplexer<F, TB, std::queue<TI, Args...>,
        enable_if_t<
            is_either_serializer<F>::value
        >>
{
    using TC = container_type_t<std::queue<TI, Args...>>;

    inline TB operator()(TB buffer, std::queue<TI, Args...>& adaptor) const
    {
        TC container;
        auto adaptor_(adaptor); // copy

        while (!adaptor_.empty())
        {
            container.push_back(std::move(adaptor_.front()));
            adaptor_.pop();
        }

        return RawSerializationMultiplexer<F, TB, TC>()(buffer, container);
    }
};

// std::priority_queue requires Compare fist :<
template <typename TB, class T>
struct RawDeserializer<TB, T,
        enable_if_t< // is_non_default_serializable_adaptor_type
            !is_serialization_copyable<T>::value &&
            !is_serialization_copyable_blacklisted<T>::value &&
            is_adaptor_type<T>::value
        >>
{
    using TC = container_type_t<T>;

    inline TB operator()(TB buffer, T& adaptor) const
    {
        TC container;

        buffer = RawSerializationMultiplexer<RawDeserializer, TB, TC>()(buffer, container);
        adaptor = std::move(T(std::move(container)));

        return buffer;
    }
};

} // namespace NAMESPACE
