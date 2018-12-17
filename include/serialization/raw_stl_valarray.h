/*

Copyright (c) 2018 MacroBull

*/

#pragma once

#include "raw.h"
#include "traits_stl_valarray.h"

namespace NAMESPACE
{

/*
 * for:
 *      std::gslice
 *
 * managed pointers
 *
 */
template <template <typename...> class  F, typename TB>
struct RawSerializationMultiplexer<F, TB, std::gslice,
        enable_if_t<
            is_either_serializer<F>::value
        >>
{
    inline TB operator()(TB buffer, std::gslice& slice) const
    {
        buffer = RawSerializationMultiplexer<F, TB, const size_t>()(
            buffer, slice.start());
        buffer = RawSerializationMultiplexer<F, TB, const std::valarray<size_t>>()(
            buffer, slice.size());
        buffer = RawSerializationMultiplexer<F, TB, const std::valarray<size_t>>()(
            buffer, slice.stride());

        return buffer;
    }
};

template <typename TB>
struct RawDeserializer<TB, std::gslice>
{
    inline TB operator()(TB buffer, std::gslice& slice) const
    {
        size_t start;
        std::valarray<size_t> size, stride;
        buffer = RawSerializationMultiplexer<RawDeserializer, TB, size_t>()(buffer, start);
        buffer = RawSerializationMultiplexer<RawDeserializer, TB, std::valarray<size_t>>()(
                buffer, size);
        buffer = RawSerializationMultiplexer<RawDeserializer, TB, std::valarray<size_t>>()(
                buffer, stride);
        slice = std::move(std::gslice(start, std::move(size), std::move(stride)));

        return buffer;
    }
};

} // namespace NAMESPACE
