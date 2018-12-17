/*

Copyright (c) 2018 MacroBull

*/

#pragma once

#include "raw.h"
#include "traits_stl_initializer_list.h"

namespace NAMESPACE
{

/*
 * for:
 *      std::initializer_list<TI, ...>
 *
 */
template <template <typename...> class  F, typename TB,
          typename TI>
struct RawSerializationMultiplexer<F, TB, std::initializer_list<TI>,
        enable_if_t<
            is_either_serializer<F>::value
        >>
{
    inline TB operator()(TB buffer, std::initializer_list<TI>& list) const
    {
        size_t size = list.size();

        buffer = RawSerializationMultiplexer<RawSerializer, TB, size_t>()(buffer, size);
        for (const auto& item: list)
        {
            buffer = RawSerializationMultiplexer<RawSerializer, TB, const TI>()(buffer, item);
        }

        return buffer;
    }
};

template <typename TB, typename TI>
struct RawDeserializer<TB, std::initializer_list<TI>>
{
    inline TB operator()(TB buffer, std::initializer_list<TI>& list) const
    {
        size_t size;

        buffer = RawSerializationMultiplexer<RawDeserializer, TB, size_t>()(buffer, size);

        auto array = new TI[size];
        for (size_t idx = 0; idx < size; ++idx)
        {
            buffer = RawSerializationMultiplexer<RawDeserializer, TB, TI>()(
                buffer, array[idx]);
        }
//        list = std::initializer_list<TI>(array, size); // private,

        return buffer;
    }
};

} // namespace NAMESPACE
