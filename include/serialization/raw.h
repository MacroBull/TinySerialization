/*

Copyright (c) 2018 MacroBull

raw serialization to char buffer[]

*/

#pragma once

#include <cstring> // memcpy
#include <utility> // std::move

#include "traits.h"

namespace NAMESPACE
{

//template <typename T>
//using Ptr = T*;

/*
 * serializer template class enabling extensiable forward delaration support
 * member operator can be inherited
 */
template <typename TB, typename TO, typename Test = void>
struct RawEmptySerializer
{
    inline TB operator()(TB buffer, TO& /*object*/) const
    {
        static_assert(is_zero_size_object<TO>::value,
                "no suitable serialization for TO found; " \
                "make TO whiltelisted in is_serialization_copyable<TO> " \
                "to invoke default serialization, " \
                "or implement it.");

        return buffer;
    }
};

template <typename TB, typename TO, typename Test = void>
struct RawSerializer:
    RawEmptySerializer<TB, TO, Test>
{
};

template <typename TB, typename TO, typename Test = void>
struct RawDrySerializer:
    RawEmptySerializer<TB, TO, Test>
{
};

template <typename TB, typename TO, typename Test = void>
struct RawDeserializer:
    RawEmptySerializer<TB, TO, Test>
{
};

template <template <typename...> class  F, typename TB = char, typename TO = char>
using is_either_serializer = conditional_or_t<
    std::is_same<F<TB, TO>, RawSerializer<TB, TO>>::value,
    std::is_same<F<TB, TO>, RawDrySerializer<TB, TO>>::value>;

template <template <typename...> class  F, typename TB, typename TO, typename Test = void>
struct RawSerializationMultiplexer
{
    inline TB operator()(TB buffer, TO& object) const
    {
        static_assert(is_serializable<TO>::value,
                "TO is not serializable, if there is any implementation, " \
                "please make TO whiltelisted in is_serializable<TO>.");

        return F<TB, TO, Test>()(buffer, object);
    }
};

template <template <typename...> class  F, typename TB, typename TO, typename Test>
struct RawSerializationMultiplexer<F, TB, const TO, Test>
{
    inline TB operator()(TB buffer, const TO& object) const
    {
        return RawSerializationMultiplexer<F, TB, TO, Test>()(buffer, const_cast<TO&>(object));
    }
};

template <typename TB, typename TO>
inline TB serialize(TB buffer, const TO& object)
{
    return RawSerializationMultiplexer<RawSerializer, TB, const TO>()(buffer, object);
}

template <typename TB, typename TO>
inline TB serialized_end(TB buffer, const TO& object)
{
    return RawSerializationMultiplexer<RawDrySerializer, TB, const TO>()(buffer, object);
}

template <typename TB, typename TO>
inline auto deserialize(TB buffer, TO& object)
    -> enable_if_t<is_serialization_copyable<TO>::value, TB>
{
    return RawSerializationMultiplexer<RawDeserializer, TB, TO>()(buffer, object);
}

template <typename TB, typename TO>
inline auto deserialize(TB buffer, TO& object)
    -> enable_if_t<
        !is_serialization_copyable<TO>::value &&
        std::is_move_constructible<TO>::value, TB>
{
    {
        TO(std::move(object)); // default clear
    }

    return RawSerializationMultiplexer<RawDeserializer, TB, TO>()(buffer, object);
}

template <typename TB, typename TO>
inline auto deserialize(TB buffer, TO& object)
    -> enable_if_t<
        !is_serialization_copyable<TO>::value &&
        !std::is_move_constructible<TO>::value &&
        std::is_default_constructible<TO>::value, TB>
{
    object = TO();
    return RawSerializationMultiplexer<RawDeserializer, TB, TO>()(buffer, object);
}

template <typename TB, typename TO>
inline auto deserialize(TB buffer, TO& object)
    -> enable_if_t<
        !is_serialization_copyable<TO>::value &&
        !std::is_move_constructible<TO>::value &&
        !std::is_default_constructible<TO>::value, TB>
{
    return RawSerializationMultiplexer<RawDeserializer, TB, TO>()(buffer, object);
}

template <typename T>
inline auto serialized_size(const T& object)
    -> decltype(std::declval<const char*>() - std::declval<const char*>())
{
    const char * const buffer = nullptr;
    return serialized_end(buffer, object) - buffer;
}

/*
 * serialization-copyable type implementation
 *
 */
template <typename TB, typename TO>
struct RawDrySerializer<TB, TO,
        enable_if_t<is_serialization_copyable<TO>::value>>
{
    inline TB operator()(TB buffer, TO& /*object*/) const
    {
        return buffer + relative_size_of<TO, decltype(*buffer)>::value;
    }
};

template <typename TB, typename TO>
struct RawSerializer<TB, TO,
        enable_if_t<is_serialization_copyable<TO>::value>>
{
    inline TB operator()(TB buffer, TO& object) const
    {
        memcpy(buffer, &object, sizeof(TO));
        return buffer + relative_size_of<TO, decltype(*buffer)>::value;
    }
};

template <typename TB, typename TO>
struct RawDeserializer<TB, TO,
        enable_if_t<is_serialization_copyable<TO>::value>>
{
    inline TB operator()(TB buffer, TO& object) const
    {
        memcpy(&object, buffer, sizeof(TO));
        return buffer + relative_size_of<TO, decltype(*buffer)>::value;
    }
};

} // namespace NAMESPACE
