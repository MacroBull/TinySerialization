/*

Copyright (c) 2018 MacroBull

serialization traits for STL types

*/

#pragma once

#include <iterator> // for std::begin, std::end, /*can be lazy included*/
#include <memory> // for std::unique_ptr

#include "traits.h"

namespace NAMESPACE
{

/*
TRAITS_DECL_CLASS_HAS_METHOD(cbegin)
TRAITS_DECL_CLASS_HAS_METHOD(cend)
TRAITS_DECL_CLASS_HAS_METHOD(begin)
TRAITS_DECL_CLASS_HAS_METHOD(end)

template <typename T>
using has_get_iterator = conditional_and_t<
        has_method_cbegin<T>::value,
        has_method_cend<T>::value>;

template <typename T>
using has_set_iterator = conditional_and_t<
        has_method_begin<T>::value,
        has_method_end<T>::value>;
*/

template <typename T, typename Test = void>
struct is_iterable: std::false_type {};

template <typename T>
struct is_iterable<T,
        enable_if_t<
            std::is_same<
                decltype(std::begin(std::declval<T>())),
                decltype(std::end(std::declval<T>()))
            >::value
        >>: std::true_type {};

// container_type
TRAITS_DECL_CLASS_HAS_TYPE(value_type)

/*
template <typename T, typename Test = void>
struct is_container_type_with_get_iterator: std::false_type {};

template <class T>
struct is_container_type_with_get_iterator<T,
        enable_if_t<
            has_type_value_type<T>::value &&
            has_get_iterator<T>::value
        >>: std::true_type {};

template <typename T, typename Test = void>
struct is_container_type_with_set_iterator: std::false_type {};

template <class T>
struct is_container_type_with_set_iterator<T,
        enable_if_t<
            has_type_value_type<T>::value &&
            has_set_iterator<T>::value
        >>: std::true_type {};

template <class T>
using is_container_type = conditional_and_t<
        is_container_type_with_get_iterator<T>::value,
        is_container_type_with_set_iterator<T>::value>;
*/

template <typename T, typename Test = void>
struct is_container_type: std::false_type {};

template <class T>
struct is_container_type<T,
        enable_if_t<
            has_type_value_type<T>::value &&
            is_iterable<T>::value
        >>: std::true_type {};

template <class T>
using value_type_t = typename T::value_type;

/*
// sequential_type
template <typename T, typename Test = void>
struct is_sequential_type_with_get_iterator: std::false_type {};

template <template <typename...> class TC, typename TI, typename ...Args>
struct is_sequential_type_with_get_iterator<TC<TI, Args...>,
        enable_if_t<
            is_container_type_with_get_iterator<TC<TI, Args...>>::value &&
            std::is_same<TI, typename TC<TI, Args...>::value_type>::value
        >>: std::true_type {};

template <typename T, typename Test = void>
struct is_sequential_type_with_set_iterator: std::false_type {};

template <template <typename...> class TC, typename TI, typename ...Args>
struct is_sequential_type_with_set_iterator<TC<TI, Args...>,
        enable_if_t<
            is_container_type_with_set_iterator<TC<TI, Args...>>::value &&
            std::is_same<TI, typename TC<TI, Args...>::value_type>::value
        >>: std::true_type {};

template <class T>
using is_sequential_type = conditional_and_t<
        is_sequential_type_with_get_iterator<T>::value,
        is_sequential_type_with_set_iterator<T>::value>;

// associative_type
TRAITS_DECL_CLASS_HAS_TYPE(key_type)
TRAITS_DECL_CLASS_HAS_TYPE(mapped_type)

template <typename T, typename Test = void>
struct is_associative_type_with_get_iterator: std::false_type {};

template <template <typename...> class TA, typename TK, typename TV, typename ...Args>
struct is_associative_type_with_get_iterator<TA<TK, TV, Args...>,
        enable_if_t<
            is_container_type_with_get_iterator<TA<TK, TV, Args...>>::value &&
            has_type_key_type<TA<TK, TV, Args...>>::value &&
            std::is_same<TK, typename TA<TK, TV, Args...>::key_type>::value &&
            has_type_mapped_type<TA<TK, TV, Args...>>::value &&
            std::is_same<TV, typename TA<TK, TV, Args...>::mapped_type>::value &&
            has_type_value_type<TA<TK, TV, Args...>>::value &&
            !std::is_same<TK, typename TA<TK, TV, Args...>::value_type>::value
        >>: std::true_type {};

template <typename T, typename Test = void>
struct is_associative_type_with_set_iterator: std::false_type {};

template <template <typename...> class TA, typename TK, typename TV, typename ...Args>
struct is_associative_type_with_set_iterator<TA<TK, TV, Args...>,
        enable_if_t<
            is_container_type_with_set_iterator<TA<TK, TV, Args...>>::value &&
            has_type_key_type<TA<TK, TV, Args...>>::value &&
            std::is_same<TK, typename TA<TK, TV, Args...>::key_type>::value &&
            has_type_mapped_type<TA<TK, TV, Args...>>::value &&
            std::is_same<TV, typename TA<TK, TV, Args...>::mapped_type>::value &&
            has_type_value_type<TA<TK, TV, Args...>>::value &&
            !std::is_same<TK, typename TA<TK, TV, Args...>::value_type>::value
        >>: std::true_type {};

template <class T>
using is_associative_type = conditional_and_t<
        is_associative_type_with_get_iterator<T>::value,
        is_associative_type_with_set_iterator<T>::value>;
*/

// adaptor_type && KeyValuePair
TRAITS_DECL_CLASS_HAS_TYPE(container_type)

template <typename T, typename Test = void>
struct is_adaptor_type: std::false_type {};

template <class T>
struct is_adaptor_type<T,
        enable_if_t<
            has_type_value_type<T>::value &&
            !is_iterable<T>::value &&
            has_type_container_type<T>::value
        >>: std::true_type {};

template <class T>
using container_type_t = typename T::container_type;

template <class T>
using is_non_default_serializable_container_type = conditional_and_t<
        !is_serialization_copyable<T>::value,
        !is_serialization_copyable_blacklisted<T>::value,
        is_container_type<T>::value>;

template <typename TK, typename TV, typename Test = void>
struct reference_pair_type
{
    TK key;
    TV value;
};

template <typename TK, typename TV>
struct reference_pair_type<TK[], TV>
{
    TK key;
    TV value;
};

template <typename TK, typename TV>
struct reference_pair_type<TK, TV[]>
{
    TK key;
    TV value;
};

template <typename TK, typename TV>
struct reference_pair_type<TK[], TV[]>
{
    TK key;
    TV value;
};

template <typename T, typename Test = void>
struct is_pair_like_type: std::false_type {};

template <template <typename...> class TP, typename TK, typename TV>
struct is_pair_like_type<TP<TK, TV>>:
    conditional_and_t<
        !is_container_type<TP<TK, TV>>::value,
        !is_adaptor_type<TP<TK, TV>>::value,
        sizeof(TP<TK, TV>) == sizeof(reference_pair_type<TK, TV>)
    > {};

// const overload for template class
template <template <typename...> class TP, typename TK, typename TV>
struct is_pair_like_type<const TP<TK, TV>>:
    conditional_and_t<
        !is_container_type<TP<TK, TV>>::value,
        !is_adaptor_type<TP<TK, TV>>::value,
        sizeof(TP<TK, TV>) == sizeof(reference_pair_type<TK, TV>)
    > {};

// detail: fast->slow capcity query
TRAITS_DECL_CLASS_HAS_METHOD_WITH_RET(size)

// detail: fast->slow capcity modify
TRAITS_DECL_CLASS_HAS_METHOD(resize)
TRAITS_DECL_CLASS_HAS_METHOD(insert)
TRAITS_DECL_CLASS_HAS_METHOD(emplace)

template <typename TC, typename TB = char, typename Test = void>
struct is_container_batch_insertable: std::false_type {};

template <class TC, typename TB>
struct is_container_batch_insertable<TC, TB,
        enable_if_t<
            is_container_type<TC>::value &&
            // TC.insert(begin, end)
            has_method_insert<TC, value_type_t<TC>*, value_type_t<TC>*>::value &&
            // TI copyable
            is_serialization_copyable<value_type_t<TC>>::value &&
            // TI size aligns with TB
            is_relative_aligned<value_type_t<TC>, TB>::value
        >>: std::true_type {};

/*
 * @@@ whitelist is_serialization_copyable:
 * fast-forward for KeyValuePair<TK, TV>, std::tuple<...Args>
 *
 */
template <template <typename...> class TP, typename TK, typename TV>
struct is_serialization_copyable<TP<TK, TV>,
        enable_if_t<
            is_pair_like_type<TP<TK, TV>>::value &&
            !std::is_same<TP<TK, TV>, std::tuple<TK, TV>>::value &&
            !std::is_same<TP<TK, TV>, std::unique_ptr<TK, TV>>::value &&
            is_serialization_copyable<TK>::value &&
            is_serialization_copyable<TV>::value
        >>: std::true_type {};

template <typename T0, typename T1, typename ...Args>
struct is_serialization_copyable<std::tuple<T0, T1, Args...>,
        enable_if_t<
            is_serialization_copyable<T0>::value &&
            is_serialization_copyable<std::tuple<T1, Args...>>::value
        >>: std::true_type {};

template <typename T0>
struct is_serialization_copyable<std::tuple<T0>,
        enable_if_t<
            is_serialization_copyable<T0>::value
        >>: std::true_type {};

/*
 * @@@ whitelist is_serializable for:
 *      KeyValuePair<TK, TV>, std::tuple<...Args>
 *
 */
template <template <typename...> class TP, typename TK, typename TV>
struct is_serializable<TP<TK, TV>,
        enable_if_t<
            is_pair_like_type<TP<TK, TV>>::value &&
            !std::is_same<TP<TK, TV>, std::tuple<TK, TV>>::value &&
            !std::is_same<TP<TK, TV>, std::unique_ptr<TK, TV>>::value &&
            is_serializable<TK>::value &&
            is_serializable<TV>::value
        >>: std::true_type {};

template <typename T0, typename T1, typename ...Args>
struct is_serializable<std::tuple<T0, T1, Args...>,
        enable_if_t<
            is_serializable<T0>::value &&
            is_serializable<std::tuple<T1, Args...>>::value
        >>: std::true_type {};

template <typename T0>
struct is_serializable<std::tuple<T0>,
        enable_if_t<
            is_serializable<T0>::value
        >>: std::true_type {};

/*
 * @@@ whitelist is_serializable for:
 *      std::vector<TI, ...>, std::deque<TI, ...>
 *      std::forward_list<TI, ...>, std::list<TI, ...>
 *      std::set<TI, ...>, std::string, std::map<TK, TV, ...> ...
 *
 */
template <class T>
struct is_serializable<T,
        enable_if_t<
            !is_serialization_copyable_blacklisted<T>::value && // specialization check
            is_container_type<T>::value &&
            is_serializable<value_type_t<T>>::value
        >>: std::true_type {};

/*
 * @@@ whitelist is_serializable for:
 *      std::stack<TI, ...>, std::queue<TI, ...>
 *      std::priority_queue<TI, Cont, Cmp, ...> not supported
 *
 */
template <class T>
struct is_serializable<T,
        enable_if_t<
            !is_serialization_copyable_blacklisted<T>::value && // specialization check
            is_adaptor_type<T>::value &&
            is_serializable<value_type_t<T>>::value &&
            is_serializable<container_type_t<T>>::value
        >>: std::true_type {};

/*
 * @@@ blacklist is_serialization_copyable for:
 *      std::unique_ptr<T>
 *
 */
template <typename T>
struct is_serialization_copyable_blacklisted<std::unique_ptr<T>>: std::true_type {};

/*
 * @@@ whitelist is_serializable for:
 *      std::unique_ptr<T>
 *
 */
template <typename T>
struct is_serializable<std::unique_ptr<T>>: is_serializable<T> {};

} // namespace NAMESPACE
