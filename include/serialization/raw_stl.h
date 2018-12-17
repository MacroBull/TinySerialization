/*

Copyright (c) 2018 MacroBull

serialization STL type extensions

the very ealy version looks like:

    template <template <typename...> class C, typename T, typename ...Ts>
    char* serialize(char* buffer, C<T, Ts...>& c)
    {
        (*(size_t*)buffer) = c.size(); buffer += sizeof(size_t);
        for (auto &i : c)
        {
            buffer = serialize(buffer, i);
        }
        return buffer;
    }

    template <template <typename...> class C, typename T, typename ...Ts>
    char* deserialize(const char* data, C<T, Ts...>& c)
    {
        c.resize(*(size_t*)data);
        auto ret = (char*)data + sizeof(size_t);
        for (auto &i : c)
        {
            ret = deserialize(ret, i);
        }
        return ret;
    }

    template <typename T1, typename T2>
    inline char* serialize(char* buffer, std::pair<T1, T2>& c)
    {
        buffer = serialize(buffer, c.first);
        buffer = serialize(buffer, c.second);
        return buffer;
    }

    template <typename T1, typename T2>
    inline char* deserialize(const char* data, std::pair<T1, T2>& c)
    {
        auto ret = (char*)data;
        ret = deserialize(ret, c.first);
        ret = deserialize(ret, c.second);
        return ret;
    }

    template <typename K, typename V, typename ...Ts>
    char* deserialize(const char* data, std::unordered_map<K, V, Ts...>& m)
    {
        auto size = (*(size_t*)data);
        auto ret = (char*)data + sizeof(size_t);
        m.reserve(size);
        while (size-- > 0)
        {
            std::pair<K, V> i;
            ret = deserialize(ret, i);
            m.emplace(std::move(i));
        }
        return ret;
    }

    template <typename T>
    inline char* serialize(char* buffer, T& i)
    {
        (*(T*)buffer) = i; buffer += sizeof(T);
        return buffer;
    }

    template <typename T>
    inline char* deserialize(const char* data, T& i)
    {
        i = (*(T*)data);
        return (char*)data + sizeof(T);
    }

*/

#pragma once

#include <cstddef>   // for size_t
#include <utility>   // for std::pair, std::tuple

#include "raw.h"
#include "traits_stl.h"

namespace NAMESPACE
{

/*
 * for:
 *      std::pair<TK, TV>
 *
 * KeyValuePair standard version
 *
 */
template <template <typename...> class  F, typename TB,
          typename TK, typename TV>
struct RawSerializationMultiplexer<F, TB, std::pair<TK, TV>,
        enable_if_t<
            !is_serialization_copyable<std::pair<TK, TV>>::value
        >>
{
    inline TB operator()(TB buffer, std::pair<TK, TV>& pair) const
    {
        buffer = RawSerializationMultiplexer<F, TB, TK>()(buffer, pair.first);
        buffer = RawSerializationMultiplexer<F, TB, TV>()(buffer, pair.second);

        return buffer;
    }
};

/*
 * for:
 *      std::tuple<...Args>
 *
 */
template <size_t I, typename T>
using tuple_element_t = typename std::tuple_element<I, T>::type;

template <template <typename...> class  F, typename TB, typename TO, size_t N>
struct RawTupleSerializationHelper
{
    static inline TB call(TB buffer, TO& tuple)
    {
        const auto i = N - 1;

        buffer = RawTupleSerializationHelper<F, TB, TO, i>::call(buffer, tuple);
        return RawSerializationMultiplexer<F, TB, tuple_element_t<i, TO>>()(
                buffer, std::get<i>(tuple));
    }
};

template <template <typename...> class  F, typename TB, typename TO>
struct RawTupleSerializationHelper<F, TB, TO, 1>
{
    static inline TB call(TB buffer, TO& tuple)
    {
        return RawSerializationMultiplexer<F, TB, tuple_element_t<0, TO>>()(
            buffer, std::get<0>(tuple));
    }
};

template <template <typename...> class  F, typename TB,
          typename ...Args>
struct RawSerializationMultiplexer<F, TB, std::tuple<Args...>,
        enable_if_t<
            !is_serialization_copyable<std::tuple<Args...>>::value
        >>
{
    using TT = std::tuple<Args...>;

    inline TB operator()(TB buffer, TT& tuple) const
    {
        return RawTupleSerializationHelper<F, TB, TT, sizeof ...(Args)>::call(
            buffer, tuple);
    }
};

/*
 * for:
 *      std::vector<TI, ...>, std::deque<TI, ...>
 *      std::set<TI, ...>, std::multiset<TI, ...>
 *      std::map<TK, TV, ...>, std::unordered_map<TK, TV, ...>
 *      std::string...
 *
 * size_t TO.size() makes serialize easier
 * serialize and dry-serialize standard version
 *
 * NOTE(): using "...Args" for MSVC-like suck compilers
 * the very early version:
 *
 *      template <template <typename...> class TC, typename TI, typename ...Args>
 *      char* serialize(char* buffer, TC<TI, Args...>& c)
 *      {
 *          (*(size_t*)buffer) = c.size(); buffer += sizeof(size_t);
 *          for (auto& i : c)
 *          {
 *              buffer = serialize(buffer, i);
 *          }
 *          return buffer;
 *      }
 *
 */
template <template <typename...> class  F, typename TB,
          class T>
struct RawSerializationMultiplexer<F, TB, T,
        enable_if_t<
            is_non_default_serializable_container_type<T>::value &&
            has_method_size_with_ret<T, size_t(void)>::value &&
            (std::is_same<F<TB, T>, RawSerializer<TB, T>>::value ||
                // non-copyable TI uses standard container serialization
                (std::is_same<F<TB, T>, RawDrySerializer<TB, T>>::value &&
                !is_serialization_copyable<value_type_t<T>>::value))
        >>
{
    using TI = value_type_t<T>;

    inline TB operator()(TB buffer, T& container) const
    {
        size_t size = container.size();

        buffer = RawSerializationMultiplexer<F, TB, size_t>()(buffer, size);
        for (const auto& item: container)
        {
            buffer = RawSerializationMultiplexer<F, TB, const TI>()(buffer, item);
        }

        return buffer;
    }
};

// dry-serialize fast-forward version
template <typename TB, class T>
struct RawDrySerializer<TB, T,
        enable_if_t<
            is_non_default_serializable_container_type<T>::value &&
            has_method_size_with_ret<T, size_t(void)>::value && // restricted to size_t
            is_serialization_copyable<value_type_t<T>>::value
        >>
{
    using TI = value_type_t<T>;

    inline TB operator()(TB buffer, T& container) const
    {
        size_t size = container.size();

        buffer = RawSerializationMultiplexer<RawDrySerializer, TB, size_t>()(
            buffer, size);
        return buffer + relative_size_of<TI, decltype(*buffer)>::value * size;
    }
};

/*
 * for:
 *      std::vector<TI, ...>, std::deque<TI, ...>
 *      std::forward_list<TI, ...>, std::list<TI, ...>
 *      std::string ...
 *
 * TO.resize(size_t) makes deserialize easier
 * standard version
 *
 */
template <typename TB, class T>
struct RawDeserializer<TB, T,
        enable_if_t<
            is_non_default_serializable_container_type<T>::value &&
            has_method_resize<T, size_t>::value // restricted to size_t
        >>
{
    using TI = value_type_t<T>;

    inline TB operator()(TB buffer, T& container) const
    {
        size_t size;

        buffer = RawSerializationMultiplexer<RawDeserializer, TB, size_t>()(buffer, size);
        container.resize(size);
        for (auto& item: container)
        {
            buffer = RawSerializationMultiplexer<RawDeserializer, TB, TI>()(buffer, item);
        }

        return buffer;
    }
};

/*
 * for:
 *      std::forward_list<TI, ...>, std::list<TI, ...>
 *
 * standard version
 *
 */
template <typename TB, class T>
struct RawSerializer<TB, T,
        enable_if_t<
            is_non_default_serializable_container_type<T>::value &&
            !has_method_size_with_ret<T, size_t(void)>::value // restricted to size_t
        >>
{
    using TI = value_type_t<T>;

    inline TB operator()(TB buffer, T& container) const
    {
        const auto size_head = buffer;

        size_t size = 0;

        buffer += relative_size_of<size_t, decltype(*buffer)>::value;
        for (const auto& item: container)
        {
            buffer = RawSerializationMultiplexer<RawSerializer, TB, const TI>()(
                buffer, item);
            ++size;
        }
        RawSerializationMultiplexer<RawSerializer, TB, size_t>()(size_head, size);

        return buffer;
    }
};

template <typename TB, class T>
struct RawDrySerializer<TB, T,
       enable_if_t<
            is_non_default_serializable_container_type<T>::value &&
            !has_method_size_with_ret<T, size_t(void)>::value && // restricted to size_t
            is_serialization_copyable<value_type_t<T>>::value
       >>
{
    using TI = value_type_t<T>;

    inline TB operator()(TB buffer, T& container) const
    {
        buffer += relative_size_of<size_t, decltype(*buffer)>::value;
        for (const auto& item: container)
        {
            buffer = RawSerializationMultiplexer<RawDrySerializer, TB, const TI>()(
                buffer, item);
        }

        return buffer;
    }
};

/*
 * for:
 *      std::set<TI, ...>, std::map<TK, TV, ...>
 *
 * fast insert version
 */
template <typename TB, class T>
struct RawDeserializer<TB, T,
        enable_if_t<
            is_non_default_serializable_container_type<T>::value &&
            !has_method_resize<T, size_t>::value && // restricted to size_t
            is_container_batch_insertable<T>::value
        >>
{
    using TI = value_type_t<T>;

    inline TB operator()(TB buffer, T& container) const
    {
        size_t size;

        buffer = RawSerializationMultiplexer<RawDeserializer, TB, size_t>()(buffer, size);

        const auto item_head = reinterpret_cast<const TI*>(buffer);

        container.insert(item_head, item_head + size);

        return buffer + relative_size_of<TI, decltype(*buffer)>::value * size;
    }
};

/*
 * for:
 *      std::set<TI, ...>, std::map<TK, TV, ...>
 *
 * standard emplace version
 */
template <typename TB, class T>
struct RawDeserializer<TB, T,
        enable_if_t<
            is_non_default_serializable_container_type<T>::value &&
            !has_method_resize<T, size_t>::value && // restricted to size_t
            !is_container_batch_insertable<T>::value &&
            has_method_emplace<T, value_type_t<T>>::value
        >>
{
    using TI = value_type_t<T>;

    inline TB operator()(TB buffer, T& container) const
    {
        size_t size;

        buffer = RawSerializationMultiplexer<RawDeserializer, TB, size_t>()(buffer, size);
        for (; size > 0; --size)
        {
            TI item;
            buffer = RawSerializationMultiplexer<RawDeserializer, TB, TI>()(buffer, item);
            container.emplace(std::move(item)); // noexcept // if move applicable
        }

        return buffer;
    }
};

/*
 * for:
 *      std::unique_ptr<T, ...>
 *
 * managed pointers
 *
 */
template <template <typename...> class  F, typename TB,
          typename T, typename ...Args>
struct RawSerializationMultiplexer<F, TB, std::unique_ptr<T, Args...>,
        enable_if_t<
            is_either_serializer<F>::value
        >>
{
    inline TB operator()(TB buffer, std::unique_ptr<T, Args...>& pointer) const
    {
        bool test = pointer.get() != nullptr;

        buffer = RawSerializationMultiplexer<F, TB, bool>()(buffer, test);
        if (test)
        {
            buffer = RawSerializationMultiplexer<F, TB, T>()(buffer, *pointer);
        }

        return buffer;
    }
};

template <typename TB, typename T, typename ...Args>
struct RawDeserializer<TB, std::unique_ptr<T, Args...>>
{
    inline TB operator()(TB buffer, std::unique_ptr<T, Args...>& pointer) const
    {
        bool test;

        buffer = RawSerializationMultiplexer<RawDeserializer, TB, bool>()(buffer, test);
        if (test)
        {
            pointer.reset(new T);
            buffer = RawSerializationMultiplexer<RawDeserializer, TB, T>()(buffer, *pointer);
        }

        return buffer;
    }
};

} // namespace NAMESPACE
