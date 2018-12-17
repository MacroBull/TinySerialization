/*

Copyright (c) 2018 MacroBull

basic traits for serialization

*/

#pragma once

#include <type_traits> // for std::... traits, /*can be lazy included*/

/*
 * class has type: T::name trait.
 *
 * implementation 1:
 *
 *      // primary template handles types that have no nested ::type member:
 *      template< class, class = std::void_t<> >
 *      struct has_type_member : std::false_type {};
 *
 *      // specialization recognizes types that do have a nested ::type member:
 *      template< class T >
 *      struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};
 *
 * implementation 2:
 *
 *      template <class T>
 *      class has_type_member
 *      {
 *          template <typename U>
 *          struct helper
 *          {
 *              using type = void;
 *          };
 *
 *          template <typename U, typename V = void>
 *          struct impl
 *          {
 *              static const bool value = false;
 *          };
 *
 *          template <typename U>
 *          struct impl<U, typename helper<typename U::name>::type>
 *          {
 *              static const bool value = true;
 *          };
 *
 *      public:
 *          static const bool value = impl<T>::value;
 *      };
 *
 */
#define TRAITS_DECL_CLASS_HAS_TYPE(name) \
    template <class T, typename Test = void> \
    struct has_type_##name: std::false_type {}; \
    template <class T> \
    struct has_type_##name<T, \
            NAMESPACE::void_t<typename T::name> \
    >: std::true_type {}; \

/*
 * class has object: T::name trait.
 *
 */
#define TRAITS_DECL_CLASS_HAS_OBJECT(name) \
    template <class T, typename Test = void> \
    struct has_object_##name: std::false_type {}; \
    template <class T> \
    struct has_object_##name<T, \
            enable_if_t< \
                !std::is_function<decltype(std::declval<T>().name)>::value, \
                NAMESPACE::void_t<decltype(std::declval<T>().name)> \
            >>: std::true_type {}; \

/*
 * class has member object: T.name trait.
 *
 * NOTE(): type0 still cannot handle a cv member object in a class reference
 */
/*
#define TRAITS_DECL_CLASS_HAS_MEMBER_OBJECT(name) \
    template <class T, typename Test = void> \
    struct has_member_object_##name##_type0: std::false_type {}; \
    template <class T> \
    struct has_member_object_##name##_type0<T, \
            enable_if_t< \
                std::is_member_object_pointer<decltype(&std::declval<T>().name)>::value \
            >>: std::true_type {}; \
    template <class T> \
    struct has_member_object_##name##_type0<T, \
            enable_if_t< \
                std::is_reference<decltype(std::declval<T>().name)>::value \
            >>: std::true_type {}; \
    template <class T, typename Test = void> \
    struct has_member_object_##name##_type1: std::false_type {}; \
    template <class T> \
    struct has_member_object_##name##_type1<T, \
            enable_if_t< \
                std::is_member_object_pointer<decltype(&T::name)>::value \
            >>: std::true_type {}; \
    template <class T> \
    struct has_member_object_##name##_type1<T, \
            enable_if_t< \
                std::is_reference<decltype(T::name)>::value \
            >>: std::true_type {}; \
    template <class T> \
    using has_member_object_##name = conditional_or_t< \
            has_member_object_##name##_type0<T>::value, \
            has_member_object_##name##_type1<T>::value>; \
 */

/*
 * class has method: T::name trait.
 *
 */
/*
#define TRAITS_DECL_CLASS_HAS_STATIC_METHOD(name) \
    template <class T, typename ...Args> \
    class has_static_method_##name \
    { \
        template <class U, typename Test = void> \
        struct helper: std::false_type {}; \
        template <class U> \
        struct helper<U, NAMESPACE::void_t<decltype(U::name())>>: std::true_type {}; \
    public: \
        static const bool value = helper<T>::value; \
    }; \
    template <class T, typename Arg0, typename ...Args> \
    class has_static_method_##name<T, Arg0, Args...> \
    { \
        template <class U, typename Test = void> \
        struct helper: std::false_type {}; \
        template <class U> \
        struct helper<U, \
                NAMESPACE::void_t< \
                    decltype(U::name(std::declval<Arg0>(), \
                    std::declval<Args>()...)) \
                > \
        >: std::true_type {}; \
    public: \
        static const bool value = helper<T>::value; \
    }; \
 */

/*
 * class has method: T.name trait matches (Args...).
 *
 */
#define TRAITS_DECL_CLASS_HAS_METHOD(name) \
    template <class T, typename ...Args> \
    class has_method_##name \
    { \
        template <class U, typename Test = void> \
        struct helper: std::false_type {}; \
        template <class U> \
        struct helper<U, \
                NAMESPACE::void_t<decltype(std::declval<U>().name())> \
        >: std::true_type {}; \
    public: \
        static const bool value = helper<T>::value; \
    }; \
    template <class T, typename Arg0, typename ...Args> \
    class has_method_##name<T, Arg0, Args...> \
    { \
        template <class U, typename Test = void> \
        struct helper: std::false_type {}; \
        template <class U> \
        struct helper<U, \
                NAMESPACE::void_t< \
                    decltype(std::declval<U>().name( \
                        std::declval<Arg0>(), std::declval<Args>()...))> \
        >: std::true_type {}; \
    public: \
        static const bool value = helper<T>::value; \
    }; \

/*
 * class has method T.name trait matches Ret(Args...).
 *
 * implementation:
 *
 *      template <typename C, typename T>
 *      struct has_name
 *      {
 *          static_assert(std::integral_constant<T, false>::value,
 *                  "Second template parameter needs to be of function type.");
 *      };
 *
 *      template <typename C, typename Ret, typename ...Args>
 *      struct has_name<C, Ret(Args...)>
 *      {
 *          template <class T>
 *          static constexpr auto check(T*)
 *              -> typename std::is_same<
 *                  decltype(std::declval<T>().name(std::declval<Args>()...)),
 *                  Ret>::type; // ^^ attempt to call it and see if the return type is correct ^^
 *
 *          template<typename>
 *          static constexpr std::false_type check(...);
 *
 *          typedef decltype(check<C>(nullptr)) type;
 *
 *      public:
 *          static constexpr bool value = type::value;
 *
 *      };
 *
 */
#define TRAITS_DECL_CLASS_HAS_METHOD_WITH_RET(name) \
    template <class T, typename Ret> \
    class has_method_##name##_with_ret: public std::false_type {}; \
    template <class T, typename Ret, typename ...Args> \
    class has_method_##name##_with_ret<T, Ret(Args...)> \
    { \
        template <class U> \
        static constexpr std::false_type check(...); \
        template <class U> \
        static constexpr auto check(remove_reference_t<U>*) -> \
                typename std::is_same< \
                    decltype(std::declval<U>().name(std::declval<Args>()...)), \
                    Ret>; \
        using type = decltype(check<T>(nullptr)); \
    public: \
        static const bool value = type::value; \
    }; \

namespace NAMESPACE
{

/*
 * make_void and void_t from C++17
 *
 */
template <typename ...Args>
struct make_void
{
    using type = void;
};

template <typename ...Args>
using void_t = typename make_void<Args...>::type;

/*
 * enable_if_t from C++17
 *
 */
template <bool Condition, typename T = void>
using enable_if_t = typename std::enable_if<Condition, T>::type;

template <bool B0, bool B1, bool ...Args>
struct conditional_and_t: conditional_and_t<B0 && B1, Args...> {};

template <bool B0, bool B1>
struct conditional_and_t<B0, B1>:
    std::conditional<B0 && B1, std::true_type, std::false_type>::type {};

template <bool B0, bool B1, bool ...Args>
struct conditional_or_t: conditional_or_t<B0 || B1, Args...> {};

template <bool B0, bool B1>
struct conditional_or_t<B0, B1>:
    std::conditional<B0 || B1, std::true_type, std::false_type>::type {};

template <bool B0, bool B1>
struct conditional_xor_t:
    std::conditional<B0 != B1, std::true_type, std::false_type>::type {};

/*
 * remove_cvref from C++20
 *
 */
template <typename T>
using remove_cv_t = typename std::remove_cv<T>::type;

template <typename T>
using remove_const_t = typename std::remove_const<T>::type;

template <typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

template <typename T>
struct remove_cvref
{
    using type = remove_cv_t<remove_reference_t<T>>;
};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

/*
 * get function type of class member method: &T::method -> method
 *
 */
template <class T>
struct remove_class_pointer {};

template <class T, typename U>
struct remove_class_pointer<U T::*>
{
    using type = U;
};

template <typename T>
using remove_class_pointer_t = typename remove_class_pointer<T>::type;

/*
 * **** basic serialization traits ****
 *
 */

//template <typename T>
//using Ptr = T*;

template <typename T, typename Test = void>
struct is_zero_size_object
{
    static const bool value = sizeof(T) == 0;
};

template <>
struct is_zero_size_object<void> // special case to skip warnings
{
    static const bool value = false;
};

template <typename T>
struct is_zero_size_object<T[]> // special case to skip errors
{
    static const bool value = true;
};

template <typename TO, typename TB = char>
struct relative_size_of
{
    static const auto value =
            is_zero_size_object<TO>::value ? 0 : ((sizeof(TO) - 1) / sizeof(TB) + 1);
};

template <typename TO, typename TB = char>
struct is_relative_aligned
{
    static const auto value =
            relative_size_of<TO, TB>::value * sizeof(TB) == sizeof(TO);
};

/*
 * default memcpy-able trait
 *
 */
template <typename T>
using is_trivially_serializable = conditional_and_t<
        std::is_trivially_copyable<T>::value,
        !std::is_pointer<T>::value>;

/*
 * @@@ extensiable trait indicating a non-trivially-copyable is serialization copyable
 *
 *      whitelist: is_serialization_copyable
 *      blacklist: is_serialization_copyable_blacklisted
 *
 * const type and flexible array specialized
 *
 */
template <typename T, typename Test = void>
struct is_serialization_copyable_blacklisted: std::false_type {};

template <typename T>
struct is_serialization_copyable_blacklisted<T[]>: std::true_type {};

template <typename T>
struct is_serialization_copyable_blacklisted<const T>: std::true_type {};

template <typename T, typename Test = void>
struct is_serialization_copyable:
    conditional_and_t<
        !is_serialization_copyable_blacklisted<T>::value,
        !is_zero_size_object<T>::value,
        is_trivially_serializable<T>::value
    > {};

template <typename T, typename Test>
struct is_serialization_copyable<const T, Test>: is_serialization_copyable<T, Test> {};

/*
 * @@@ extensiable trait indicating a specialization for a type has be implemented
 *
 *      whitelist: is_serializable
 *      blacklist: is_serializable_blacklisted
 *
 * flexible array specialized
 *
 */
template <typename T, typename Test = void>
struct is_serializable_blacklisted: std::false_type {};

template <typename T>
struct is_serializable_blacklisted<T[]>: std::true_type {};

template <typename T>
struct is_serializable_blacklisted<const T>: std::true_type {};

template <typename T, typename Test = void>
struct is_serializable:
        conditional_and_t<
            !is_serializable_blacklisted<T>::value,
            is_serialization_copyable<T>::value
        > {};

template <typename T, typename Test>
struct is_serializable<const T, Test>: is_serializable<T, Test> {};

} // namespace NAMESPACE
