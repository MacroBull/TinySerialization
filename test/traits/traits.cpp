/*

Copyleft 2018 Macrobull

*/

#include <memory>
#include <stack>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "serialization/traits.h"

#include "test.h"

namespace NAMESPACE
{

enum Enum
{
    Apple,
    Banana,
    Coconut,
};

union Union
{
    int int_value;
    float float_value;
    double double_value;
};

struct Struct
{
    using value_type = int;

    const std::vector<int> const_value = { 1, 2, 3 };
    static constexpr int constexpr_value = 42;

    volatile std::pair<int, void*> volatile_object;
    static Enum enum_object;
    Union union_object;
    Struct& reference_object = *this;
    const Struct* const_pointer_object = this;

    static void static_method();
    size_t member_method() const;
    size_t member_method(int);
    size_t member_method(float);
    size_t member_method2(float);

    virtual bool virtual_method();
    virtual int pure_virtual_method() = delete;

    Struct& operator+(const Struct&);

    template <typename T>
    T template_method(const T&);

private:
    const Struct* private_const_pointer_object = this;

    inline size_t private_method();

};

using ZeroType = char[0];
using FunctionType = size_t(int*, float);
using FunctorType = std::less<int>;

using void_type = void;
using zero_type = ZeroType;
using trivial_type = bool;
using pointer_type = char*;
using enum_type = Enum;
using union_type = Union;
using pair_type = std::pair<int, float>;
using vector_type = std::vector<double>;
using const_type = const std::vector<void*>;
using reference_type = std::vector<char>& ;
using volatile_type = volatile std::vector<float>;
using const_object_type = decltype(std::declval<Struct>().const_value);
using constexpr_object_type = decltype(Struct::constexpr_value);
using volatile_object_type = decltype(std::declval<Struct>().volatile_object);
using static_enum_object_type = decltype(Struct::enum_object);
using union_object_type = decltype(std::declval<Struct>().union_object);
using reference_object_type = decltype(std::declval<Struct>().reference_object);
using static_method_type = decltype(Struct::static_method);
using member_method_type = decltype(&Struct::member_method2);
using virtual_method_type = decltype(&Struct::virtual_method);
using member_operator_type = decltype(&Struct::operator+);
using member_template_method_type = decltype(&Struct::template_method<int>);

TEST(Macros, Concatenate)
{
#define SUPER_WHAT_BOY(name) super_##name##_boy

    auto super_meat_boy = "SuperMeatBoy";

    LOG() << "SUPER_WHAT_BOY(meat)? " << SUPER_WHAT_BOY(meat) << std::endl;

#undef SUPER_WHAT_BOY
}

TRAITS_DECL_CLASS_HAS_TYPE(value_type)

TEST(Traits, HasType)
{
    // T& is not a class, add remove_reference
//    using T = std::vector<char>&;
//    using U = remove_reference_t<T>::value_type;

    auto void_result = has_type_value_type<void_type>::value;
    auto zero_result = has_type_value_type<zero_type>::value;
    auto trivial_result = has_type_value_type<trivial_type>::value;
    auto pointer_result = has_type_value_type<pointer_type>::value;
    auto enum_result = has_type_value_type<enum_type>::value;
    auto union_result = has_type_value_type<union_type>::value;
    auto pair_result = has_type_value_type<pair_type>::value;
    auto vector_result = has_type_value_type<vector_type>::value;
    auto const_result = has_type_value_type<const_type>::value;
    auto reference_result = has_type_value_type<reference_type>::value;
    auto volatile_result = has_type_value_type<volatile_type>::value;
    auto const_object_result = has_type_value_type<const_object_type>::value;
    auto constexpr_object_result =
        has_type_value_type<constexpr_object_type>::value;
    auto volatile_object_result = has_type_value_type<volatile_object_type>::value;
    auto static_enum_object_result =
        has_type_value_type<static_enum_object_type>::value;
    auto union_object_result = has_type_value_type<union_object_type>::value;
    auto reference_object_result =
        has_type_value_type<reference_object_type>::value;
    auto static_method_result = has_type_value_type<static_method_type>::value;
    auto member_method_result = has_type_value_type<member_method_type>::value;
    auto virtual_method_result = has_type_value_type<virtual_method_type>::value;
    auto member_operator_result = has_type_value_type<member_operator_type>::value;
    auto member_template_method_result =
        has_type_value_type<member_template_method_type>::value;

    EXPECT_EQ(void_result, false);
    EXPECT_EQ(zero_result, false);
    EXPECT_EQ(trivial_result, false);
    EXPECT_EQ(pointer_result, false);
    EXPECT_EQ(enum_result, false);
    EXPECT_EQ(union_result, false);
    EXPECT_EQ(pair_result, false);
    EXPECT_EQ(vector_result, true);
    EXPECT_EQ(const_result, true);
    EXPECT_EQ(reference_result, false); // <<< !important
    EXPECT_EQ(volatile_result, true);
    EXPECT_EQ(const_object_result, true);
    EXPECT_EQ(constexpr_object_result, false);
    EXPECT_EQ(volatile_object_result, false);
    EXPECT_EQ(static_enum_object_result, false);
    EXPECT_EQ(union_object_result, false);
    EXPECT_EQ(reference_object_result, false); // <<< !important
    EXPECT_EQ(static_method_result, false);
    EXPECT_EQ(member_method_result, false);
    EXPECT_EQ(virtual_method_result, false);
    EXPECT_EQ(member_operator_result, false);
    EXPECT_EQ(member_template_method_result, false);
}

TRAITS_DECL_CLASS_HAS_OBJECT(const_value)
TRAITS_DECL_CLASS_HAS_OBJECT(constexpr_value)
TRAITS_DECL_CLASS_HAS_OBJECT(volatile_object)
TRAITS_DECL_CLASS_HAS_OBJECT(enum_object)
TRAITS_DECL_CLASS_HAS_OBJECT(union_object)
TRAITS_DECL_CLASS_HAS_OBJECT(reference_object)
TRAITS_DECL_CLASS_HAS_OBJECT(static_method)
TRAITS_DECL_CLASS_HAS_OBJECT(member_method)
TRAITS_DECL_CLASS_HAS_OBJECT(value_type)
TRAITS_DECL_CLASS_HAS_OBJECT(const_pointer_object)
TRAITS_DECL_CLASS_HAS_OBJECT(private_const_pointer_object)

TEST(Traits, HasObject)
{
    {
        using Type = Struct;

        auto const_object_result = has_object_const_value<Type>::value;
        auto constexpr_object_result = has_object_constexpr_value<Type>::value;
        auto volatile_object_result = has_object_volatile_object<Type>::value;
        auto static_enum_object_result = has_object_enum_object<Type>::value;
        auto union_object_result = has_object_union_object<Type>::value;
        auto reference_object_result = has_object_reference_object<Type>::value;
        auto static_method_result = has_object_static_method<Type>::value;
        auto member_method_result = has_object_member_method<Type>::value;
        auto value_type_result = has_object_value_type<Type>::value;
        auto const_pointer_object_result = has_object_const_pointer_object<Type>::value;
//        auto private_object_result = has_object_private_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, true);
        EXPECT_EQ(constexpr_object_result, true);
        EXPECT_EQ(volatile_object_result, true);
        EXPECT_EQ(static_enum_object_result, true);
        EXPECT_EQ(union_object_result, true);
        EXPECT_EQ(reference_object_result, true);
        EXPECT_EQ(static_method_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, true);
//        EXPECT_EQ(private_object_result, true); // <<< !important
    }

    {
        using Type = const Struct;

        auto const_object_result = has_object_const_value<Type>::value;
        auto constexpr_object_result = has_object_constexpr_value<Type>::value;
        auto volatile_object_result = has_object_volatile_object<Type>::value;
        auto static_enum_object_result = has_object_enum_object<Type>::value;
        auto union_object_result = has_object_union_object<Type>::value;
        auto reference_object_result = has_object_reference_object<Type>::value;
        auto static_method_result = has_object_static_method<Type>::value;
        auto member_method_result = has_object_member_method<Type>::value;
        auto value_type_result = has_object_value_type<Type>::value;
        auto const_pointer_object_result = has_object_const_pointer_object<Type>::value;
//        auto private_object_result = has_object_private_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, true);
        EXPECT_EQ(constexpr_object_result, true);
        EXPECT_EQ(volatile_object_result, true);
        EXPECT_EQ(static_enum_object_result, true);
        EXPECT_EQ(union_object_result, true);
        EXPECT_EQ(reference_object_result, true);
        EXPECT_EQ(static_method_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, true);
//        EXPECT_EQ(private_object_result, true); // <<< !important
    }

    {
        using Type = Struct&; // <<< !important

        auto const_object_result = has_object_const_value<Type>::value;
        auto constexpr_object_result = has_object_constexpr_value<Type>::value;
        auto volatile_object_result = has_object_volatile_object<Type>::value;
        auto static_enum_object_result = has_object_enum_object<Type>::value;
        auto union_object_result = has_object_union_object<Type>::value;
        auto reference_object_result = has_object_reference_object<Type>::value;
        auto static_method_result = has_object_static_method<Type>::value;
        auto member_method_result = has_object_member_method<Type>::value;
        auto value_type_result = has_object_value_type<Type>::value;
        auto const_pointer_object_result = has_object_const_pointer_object<Type>::value;
//        auto private_object_result = has_object_private_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, true);
        EXPECT_EQ(constexpr_object_result, true);
        EXPECT_EQ(volatile_object_result, true);
        EXPECT_EQ(static_enum_object_result, true);
        EXPECT_EQ(union_object_result, true);
        EXPECT_EQ(reference_object_result, true);
        EXPECT_EQ(static_method_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, true);
//        EXPECT_EQ(private_object_result, false); // <<< !important
    }

    {
        using Type = const volatile Struct&; // <<< !important, inaccessable

//        using T = Type;
//        using U = remove_cvref_t<const volatile Struct&>;

        auto const_object_result = has_object_const_value<Type>::value;
        auto constexpr_object_result = has_object_constexpr_value<Type>::value;
        auto volatile_object_result = has_object_volatile_object<Type>::value;
        auto static_enum_object_result = has_object_enum_object<Type>::value;
        auto union_object_result = has_object_union_object<Type>::value;
        auto reference_object_result = has_object_reference_object<Type>::value;
        auto static_method_result = has_object_static_method<Type>::value;
        auto member_method_result = has_object_member_method<Type>::value;
        auto value_type_result = has_object_value_type<Type>::value;
        auto const_pointer_object_result = has_object_const_pointer_object<Type>::value;
//        auto private_object_result = has_object_private_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, true);
        EXPECT_EQ(constexpr_object_result, true);
        EXPECT_EQ(volatile_object_result, true);
        EXPECT_EQ(static_enum_object_result, true);
        EXPECT_EQ(union_object_result, true);
        EXPECT_EQ(reference_object_result, true);
        EXPECT_EQ(static_method_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, true);
//        EXPECT_EQ(private_object_result, false); // <<< !important
    }
}

/*
TRAITS_DECL_CLASS_HAS_MEMBER_OBJECT(const_value)
TRAITS_DECL_CLASS_HAS_MEMBER_OBJECT(constexpr_value)
TRAITS_DECL_CLASS_HAS_MEMBER_OBJECT(volatile_object)
TRAITS_DECL_CLASS_HAS_MEMBER_OBJECT(enum_object)
TRAITS_DECL_CLASS_HAS_MEMBER_OBJECT(union_object)
TRAITS_DECL_CLASS_HAS_MEMBER_OBJECT(reference_object)
TRAITS_DECL_CLASS_HAS_MEMBER_OBJECT(static_method)
TRAITS_DECL_CLASS_HAS_MEMBER_OBJECT(member_method)
TRAITS_DECL_CLASS_HAS_MEMBER_OBJECT(value_type)
TRAITS_DECL_CLASS_HAS_MEMBER_OBJECT(const_pointer_object)
TRAITS_DECL_CLASS_HAS_MEMBER_OBJECT(private_const_pointer_object)

TEST(Traits, HasMemberObject)
{
    // error: cannot create pointer to reference member Struct::reference_object’
//    using T = decltype(Struct::reference_object);
//    auto t = std::is_reference<T>::value;

    {
        using Type = Struct;

        auto const_object_result = has_member_object_const_value<Type>::value;
        auto constexpr_object_result = has_member_object_constexpr_value<Type>::value;
        auto volatile_object_result = has_member_object_volatile_object<Type>::value;
        auto static_enum_object_result = has_member_object_enum_object<Type>::value;
        auto union_object_result = has_member_object_union_object<Type>::value;
        auto reference_object_result = has_member_object_reference_object<Type>::value;
        auto static_method_result = has_member_object_static_method<Type>::value;
        auto member_method_result = has_member_object_member_method<Type>::value;
        auto value_type_result = has_member_object_value_type<Type>::value;
        auto const_pointer_object_result = has_member_object_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, true);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, true);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, true);
        EXPECT_EQ(reference_object_result, true); // <<< !important
        EXPECT_EQ(static_method_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, true);
    }

    {
        using Type = const Struct;

        auto const_object_result = has_member_object_const_value<Type>::value;
        auto constexpr_object_result = has_member_object_constexpr_value<Type>::value;
        auto volatile_object_result = has_member_object_volatile_object<Type>::value;
        auto static_enum_object_result = has_member_object_enum_object<Type>::value;
        auto union_object_result = has_member_object_union_object<Type>::value;
        auto reference_object_result = has_member_object_reference_object<Type>::value;
        auto static_method_result = has_member_object_static_method<Type>::value;
        auto member_method_result = has_member_object_member_method<Type>::value;
        auto value_type_result = has_member_object_value_type<Type>::value;
        auto const_pointer_object_result = has_member_object_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, true);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, true);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, true);
        EXPECT_EQ(reference_object_result, true); // <<< !important
        EXPECT_EQ(static_method_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, true);
    }

    {
        using Type = Struct&; // <<< !important

        auto const_object_result = has_member_object_const_value<Type>::value;
        auto constexpr_object_result = has_member_object_constexpr_value<Type>::value;
        auto volatile_object_result = has_member_object_volatile_object<Type>::value;
        auto static_enum_object_result = has_member_object_enum_object<Type>::value;
        auto union_object_result = has_member_object_union_object<Type>::value;
        auto reference_object_result = has_member_object_reference_object<Type>::value;
        auto static_method_result = has_member_object_static_method<Type>::value;
        auto member_method_result = has_member_object_member_method<Type>::value;
        auto value_type_result = has_member_object_value_type<Type>::value;
        auto const_pointer_object_result = has_member_object_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, true);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, true);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, true);
        EXPECT_EQ(reference_object_result, true);
        EXPECT_EQ(static_method_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, true);
    }

    {
        using Type = const volatile Struct&; // <<< !important

        auto const_object_result = has_member_object_const_value<Type>::value;
        auto constexpr_object_result = has_member_object_constexpr_value<Type>::value;
        auto volatile_object_result = has_member_object_volatile_object<Type>::value;
        auto static_enum_object_result = has_member_object_enum_object<Type>::value;
        auto union_object_result = has_member_object_union_object<Type>::value;
        auto reference_object_result = has_member_object_reference_object<Type>::value;
        auto static_method_result = has_member_object_static_method<Type>::value;
        auto member_method_result = has_member_object_member_method<Type>::value;
        auto value_type_result = has_member_object_value_type<Type>::value;
        auto const_pointer_object_result = has_member_object_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, true);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, true);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, true);
        EXPECT_EQ(reference_object_result, true); // <<< !important
        EXPECT_EQ(static_method_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, true);
    }
}

TRAITS_DECL_CLASS_HAS_STATIC_METHOD(const_value)
TRAITS_DECL_CLASS_HAS_STATIC_METHOD(constexpr_value)
TRAITS_DECL_CLASS_HAS_STATIC_METHOD(volatile_object)
TRAITS_DECL_CLASS_HAS_STATIC_METHOD(enum_object)
TRAITS_DECL_CLASS_HAS_STATIC_METHOD(union_object)
TRAITS_DECL_CLASS_HAS_STATIC_METHOD(reference_object)
TRAITS_DECL_CLASS_HAS_STATIC_METHOD(static_method)
TRAITS_DECL_CLASS_HAS_STATIC_METHOD(member_method)
TRAITS_DECL_CLASS_HAS_STATIC_METHOD(value_type)
TRAITS_DECL_CLASS_HAS_STATIC_METHOD(const_pointer_object)
TRAITS_DECL_CLASS_HAS_STATIC_METHOD(private_const_pointer_object)

TEST(Traits, HasStaticMethod)
{
    {
        using Type = Struct;

        auto const_object_result = has_static_method_const_value<Type>::value;
        auto constexpr_object_result = has_static_method_constexpr_value<Type>::value;
        auto volatile_object_result = has_static_method_volatile_object<Type>::value;
        auto static_enum_object_result = has_static_method_enum_object<Type>::value;
        auto union_object_result = has_static_method_union_object<Type>::value;
        auto reference_object_result = has_static_method_reference_object<Type>::value;
        auto static_method_result = has_static_method_static_method<Type>::value;
        auto member_method_result = has_static_method_member_method<Type>::value;
        auto value_type_result = has_static_method_value_type<Type>::value;
        auto const_pointer_object_result = has_static_method_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, false);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, false);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, false);
        EXPECT_EQ(reference_object_result, false);
        EXPECT_EQ(static_method_result, true);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, false);
    }

    {
        using Type = const Struct;

        auto const_object_result = has_static_method_const_value<Type>::value;
        auto constexpr_object_result = has_static_method_constexpr_value<Type>::value;
        auto volatile_object_result = has_static_method_volatile_object<Type>::value;
        auto static_enum_object_result = has_static_method_enum_object<Type>::value;
        auto union_object_result = has_static_method_union_object<Type>::value;
        auto reference_object_result = has_static_method_reference_object<Type>::value;
        auto static_method_result = has_static_method_static_method<Type>::value;
        auto member_method_result = has_static_method_member_method<Type>::value;
        auto value_type_result = has_static_method_value_type<Type>::value;
        auto const_pointer_object_result = has_static_method_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, false);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, false);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, false);
        EXPECT_EQ(reference_object_result, false);
        EXPECT_EQ(static_method_result, true);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, false);
    }

    {
        using Type = Struct&; // <<< !important

        auto const_object_result = has_static_method_const_value<Type>::value;
        auto constexpr_object_result = has_static_method_constexpr_value<Type>::value;
        auto volatile_object_result = has_static_method_volatile_object<Type>::value;
        auto static_enum_object_result = has_static_method_enum_object<Type>::value;
        auto union_object_result = has_static_method_union_object<Type>::value;
        auto reference_object_result = has_static_method_reference_object<Type>::value;
        auto static_method_result = has_static_method_static_method<Type>::value;
        auto member_method_result = has_static_method_member_method<Type>::value;
        auto value_type_result = has_static_method_value_type<Type>::value;
        auto const_pointer_object_result = has_static_method_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, false);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, false);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, false);
        EXPECT_EQ(reference_object_result, false);
        EXPECT_EQ(static_method_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, false);
    }

    {
        using Type = const volatile Struct&; // <<< !important

        auto const_object_result = has_static_method_const_value<Type>::value;
        auto constexpr_object_result = has_static_method_constexpr_value<Type>::value;
        auto volatile_object_result = has_static_method_volatile_object<Type>::value;
        auto static_enum_object_result = has_static_method_enum_object<Type>::value;
        auto union_object_result = has_static_method_union_object<Type>::value;
        auto reference_object_result = has_static_method_reference_object<Type>::value;
        auto static_method_result = has_static_method_static_method<Type>::value;
        auto member_method_result = has_static_method_member_method<Type>::value;
        auto value_type_result = has_static_method_value_type<Type>::value;
        auto const_pointer_object_result = has_static_method_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, false);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, false);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, false);
        EXPECT_EQ(reference_object_result, false);
        EXPECT_EQ(static_method_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, false);
    }
}
*/

TRAITS_DECL_CLASS_HAS_METHOD(const_value)
TRAITS_DECL_CLASS_HAS_METHOD(constexpr_value)
TRAITS_DECL_CLASS_HAS_METHOD(volatile_object)
TRAITS_DECL_CLASS_HAS_METHOD(enum_object)
TRAITS_DECL_CLASS_HAS_METHOD(union_object)
TRAITS_DECL_CLASS_HAS_METHOD(reference_object)
TRAITS_DECL_CLASS_HAS_METHOD(static_method)
TRAITS_DECL_CLASS_HAS_METHOD(member_method)
TRAITS_DECL_CLASS_HAS_METHOD(value_type)
TRAITS_DECL_CLASS_HAS_METHOD(const_pointer_object)
TRAITS_DECL_CLASS_HAS_METHOD(private_const_pointer_object)

TEST(Traits, HasMethod)
{
    {
        using Type = Struct;

        auto const_object_result = has_method_const_value<Type>::value;
        auto constexpr_object_result = has_method_constexpr_value<Type>::value;
        auto volatile_object_result = has_method_volatile_object<Type>::value;
        auto static_enum_object_result = has_method_enum_object<Type>::value;
        auto union_object_result = has_method_union_object<Type>::value;
        auto reference_object_result = has_method_reference_object<Type>::value;
        auto static_method_result = has_method_static_method<Type>::value;
        auto member_method_result = has_method_member_method<Type>::value;
        auto member_method1_result = has_method_member_method<Type, int>::value;
        auto value_type_result = has_method_value_type<Type>::value;
        auto const_pointer_object_result = has_method_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, false);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, false);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, false);
        EXPECT_EQ(reference_object_result, false);
        EXPECT_EQ(static_method_result, true);
        EXPECT_EQ(member_method_result, true);
        EXPECT_EQ(member_method1_result, true);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, false);
    }

    {
        using Type = const Struct;

        auto const_object_result = has_method_const_value<Type>::value;
        auto constexpr_object_result = has_method_constexpr_value<Type>::value;
        auto volatile_object_result = has_method_volatile_object<Type>::value;
        auto static_enum_object_result = has_method_enum_object<Type>::value;
        auto union_object_result = has_method_union_object<Type>::value;
        auto reference_object_result = has_method_reference_object<Type>::value;
        auto static_method_result = has_method_static_method<Type>::value;
        auto member_method_result = has_method_member_method<Type>::value;
        auto member_method1_result = has_method_member_method<Type, int>::value;
        auto value_type_result = has_method_value_type<Type>::value;
        auto const_pointer_object_result = has_method_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, false);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, false);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, false);
        EXPECT_EQ(reference_object_result, false);
        EXPECT_EQ(static_method_result, true);
        EXPECT_EQ(member_method_result, true);
        EXPECT_EQ(member_method1_result, false); // const call const only
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, false);
    }

    {
        using Type = Struct&; // <<< !important, reference function call available!!!

        auto const_object_result = has_method_const_value<Type>::value;
        auto constexpr_object_result = has_method_constexpr_value<Type>::value;
        auto volatile_object_result = has_method_volatile_object<Type>::value;
        auto static_enum_object_result = has_method_enum_object<Type>::value;
        auto union_object_result = has_method_union_object<Type>::value;
        auto reference_object_result = has_method_reference_object<Type>::value;
        auto static_method_result = has_method_static_method<Type>::value;
        auto member_method_result = has_method_member_method<Type>::value;
        auto member_method1_result = has_method_member_method<Type, int>::value;
        auto value_type_result = has_method_value_type<Type>::value;
        auto const_pointer_object_result = has_method_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, false);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, false);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, false);
        EXPECT_EQ(reference_object_result, false);
        EXPECT_EQ(static_method_result, true);
        EXPECT_EQ(member_method_result, true);
        EXPECT_EQ(member_method1_result, true);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, false);
    }

    {
        using Type = const Struct&; // <<< !important

        auto const_object_result = has_method_const_value<Type>::value;
        auto constexpr_object_result = has_method_constexpr_value<Type>::value;
        auto volatile_object_result = has_method_volatile_object<Type>::value;
        auto static_enum_object_result = has_method_enum_object<Type>::value;
        auto union_object_result = has_method_union_object<Type>::value;
        auto reference_object_result = has_method_reference_object<Type>::value;
        auto static_method_result = has_method_static_method<Type>::value;
        auto member_method_result = has_method_member_method<Type>::value;
        auto member_method1_result = has_method_member_method<Type, int>::value;
        auto value_type_result = has_method_value_type<Type>::value;
        auto const_pointer_object_result = has_method_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, false);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, false);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, false);
        EXPECT_EQ(reference_object_result, false);
        EXPECT_EQ(static_method_result, true);
        EXPECT_EQ(member_method_result, true);
        EXPECT_EQ(member_method1_result, false); // const call const only
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, false);
    }

    {
        using Type = const volatile Struct&; // <<< !important, const volatile

        auto const_object_result = has_method_const_value<Type>::value;
        auto constexpr_object_result = has_method_constexpr_value<Type>::value;
        auto volatile_object_result = has_method_volatile_object<Type>::value;
        auto static_enum_object_result = has_method_enum_object<Type>::value;
        auto union_object_result = has_method_union_object<Type>::value;
        auto reference_object_result = has_method_reference_object<Type>::value;
        auto static_method_result = has_method_static_method<Type>::value;
        auto member_method_result = has_method_member_method<Type>::value;
        auto member_method1_result = has_method_member_method<Type, int>::value;
        auto value_type_result = has_method_value_type<Type>::value;
        auto const_pointer_object_result = has_method_const_pointer_object<Type>::value;

        EXPECT_EQ(const_object_result, false);
        EXPECT_EQ(constexpr_object_result, false);
        EXPECT_EQ(volatile_object_result, false);
        EXPECT_EQ(static_enum_object_result, false);
        EXPECT_EQ(union_object_result, false);
        EXPECT_EQ(reference_object_result, false);
        EXPECT_EQ(static_method_result,
                  false); // <<< HINT(): !important, but static works !!!???
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(member_method1_result, false);
        EXPECT_EQ(value_type_result, false);
        EXPECT_EQ(const_pointer_object_result, false);
    }
}

TRAITS_DECL_CLASS_HAS_METHOD_WITH_RET(static_method)
TRAITS_DECL_CLASS_HAS_METHOD_WITH_RET(member_method)
TRAITS_DECL_CLASS_HAS_METHOD_WITH_RET(member_method2)

TEST(Traits, HasMethodWithRet)
{
    {
        using Type = Struct;

        auto static_method_result =
            has_method_static_method_with_ret<Type, void(void)>::value;
        auto static_method1_result =
            has_method_static_method_with_ret<Type, size_t(void)>::value;
        auto member_method_result =
            has_method_member_method_with_ret<Type, void(void)>::value;
        auto member_method1_result =
            has_method_member_method_with_ret<Type, size_t(void)>::value;
        auto member_method2_result =
            has_method_member_method_with_ret<Type, size_t(float)>::value;
        auto member_method3_result =
            has_method_member_method_with_ret<Type, void(float)>::value;

        EXPECT_EQ(static_method_result, true);
        EXPECT_EQ(static_method1_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(member_method1_result, true);
        EXPECT_EQ(member_method2_result, true);
        EXPECT_EQ(member_method3_result, false);
    }

    {
        using Type = const Struct;

        auto static_method_result =
            has_method_static_method_with_ret<Type, void(void) const>::value;
        auto static_method1_result =
            has_method_static_method_with_ret<Type, size_t(void)>::value;
        auto member_method_result =
            has_method_member_method_with_ret<Type, void(void)>::value;
        auto member_method1_result =
            has_method_member_method_with_ret<Type, size_t(void)>::value;
        auto member_method2_result =
            has_method_member_method_with_ret<Type, size_t(float)>::value;
        auto member_method3_result =
            has_method_member_method_with_ret<Type, void(float)>::value;

        EXPECT_EQ(static_method_result, false);
        EXPECT_EQ(static_method1_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(member_method1_result, true); // const only
        EXPECT_EQ(member_method2_result, false);
        EXPECT_EQ(member_method3_result, false);
    }

    {
        using Type = Struct&; // <<< !important, reference function call available!!!

        auto static_method_result =
            has_method_static_method_with_ret<Type, void(void)>::value;
        auto static_method1_result =
            has_method_static_method_with_ret<Type, size_t(void)>::value;
        auto member_method_result =
            has_method_member_method_with_ret<Type, void(void)>::value;
        auto member_method1_result =
            has_method_member_method_with_ret<Type, size_t(void)>::value;
        auto member_method2_result =
            has_method_member_method_with_ret<Type, size_t(float)>::value;
        auto member_method3_result =
            has_method_member_method_with_ret<Type, void(float)>::value;

        EXPECT_EQ(static_method_result, true);
        EXPECT_EQ(static_method1_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(member_method1_result, true);
        EXPECT_EQ(member_method2_result, true);
        EXPECT_EQ(member_method3_result, false);
    }

    {
        using Type = const Struct&; // <<< !important

        auto static_method_result =
            has_method_static_method_with_ret<Type, void(void) const>::value;
        auto static_method1_result =
            has_method_static_method_with_ret<Type, size_t(void)>::value;
        auto member_method_result =
            has_method_member_method_with_ret<Type, void(void)>::value;
        auto member_method1_result =
            has_method_member_method_with_ret<Type, size_t(void)>::value;
        auto member_method2_result =
            has_method_member_method_with_ret<Type, size_t(float)>::value;
        auto member_method3_result =
            has_method_member_method_with_ret<Type, void(float)>::value;

        EXPECT_EQ(static_method_result, false);
        EXPECT_EQ(static_method1_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(member_method1_result, true); // const only
        EXPECT_EQ(member_method2_result, false);
        EXPECT_EQ(member_method3_result, false);
    }

    {
        using Type = const volatile Struct&; // <<< !important, const volatile

        auto static_method_result =
            has_method_static_method_with_ret<Type, void(void) const>::value;
        auto static_method1_result =
            has_method_static_method_with_ret<Type, size_t(void)>::value;
        auto member_method_result =
            has_method_member_method_with_ret<Type, void(void)>::value;
        auto member_method1_result =
            has_method_member_method_with_ret<Type, size_t(void)>::value;
        auto member_method2_result =
            has_method_member_method_with_ret<Type, size_t(float)>::value;
        auto member_method3_result =
            has_method_member_method_with_ret<Type, void(float)>::value;

        EXPECT_EQ(static_method_result, false); // TODO(): what it should be ???
        EXPECT_EQ(static_method1_result, false);
        EXPECT_EQ(member_method_result, false);
        EXPECT_EQ(member_method1_result, false);
        EXPECT_EQ(member_method2_result, false);
        EXPECT_EQ(member_method3_result, false);
    }
}

TEST(Traits, RemoveMember)
{
    using bond_type = decltype(&std::vector<int>::size);
    using func_type = remove_class_pointer_t<bond_type>;
    using static_type = decltype(Struct::static_method);

    auto bond_result =
        std::is_same<bond_type, size_t(std::vector<int>::*)() const>::value;
    auto func_result = std::is_same<func_type, size_t(void) const noexcept>::value;
    auto static_result = std::is_same<static_type, void(void)>::value;

    LOG() << "bond_type.typeid.name: " << typeid(bond_type).name() << std::endl;
//    LOG() << "func_type.typeid.name: " << typeid(func_type).name() << std::endl; // func type?
    LOG() << "static_type.typeid.name: " << typeid(static_type).name() << std::endl;
    LOG() << "bond_type is_member_function_pointer: " <<
          std::is_member_function_pointer<bond_type>::value << std::endl;
    LOG() << "static_type is_member_function_pointer: " <<
          std::is_member_function_pointer<static_type>::value << std::endl;

    EXPECT_EQ(bond_result, true);
    EXPECT_EQ(func_result, true);
    EXPECT_EQ(static_result, true);
}

TEST(Traits, IsTrivial)
{
    struct Struct {};
    struct StructX
    {
        std::vector<int> _;
    };

    class DefaultCopyConstruct
    {
        int _;
    public:
        DefaultCopyConstruct(const DefaultCopyConstruct&) = default;
    };

    class OverloadedCopyConstruct
    {
        int _;
    public:
        OverloadedCopyConstruct(const OverloadedCopyConstruct&)
        {
        }
    };

    class DefaultCopyAssignment
    {
        int _;
    public:
        DefaultCopyAssignment& operator=(const DefaultCopyAssignment&) = default;
    };

    class OverloadedCopyAssignment
    {
        int _;
    public:
        OverloadedCopyAssignment& operator=(const OverloadedCopyAssignment&)
        {
            return *this;
        }
    };

    char s0[0];
    char s5[5];

    {
        auto int_result = std::is_trivial<int>::value;
        auto struct_result = std::is_trivial<Struct>::value;
        auto structx_result = std::is_trivial<StructX>::value;
        auto s0_result = std::is_trivial<decltype(s0)>::value;
        auto s5_result = std::is_trivial<decltype(s5)>::value;
        auto pair_result = std::is_trivial<std::pair<int, Struct>>::value;
        auto vector_result = std::is_trivial<std::vector<int>>::value;
        auto d1 = std::is_trivial<DefaultCopyConstruct>::value;
        auto o1 = std::is_trivial<OverloadedCopyConstruct>::value;
        auto d2 = std::is_trivial<DefaultCopyAssignment>::value;
        auto o2 = std::is_trivial<OverloadedCopyAssignment>::value;
        auto p1 = std::is_trivial<int*>::value;
        auto p2 = std::is_trivial<int[]>::value;

        EXPECT_EQ(int_result, true);
        EXPECT_EQ(struct_result, true);
        EXPECT_EQ(structx_result, false);
        EXPECT_EQ(s0_result, true);
        EXPECT_EQ(s5_result, true);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(vector_result, false);
        EXPECT_EQ(d1, false);
        EXPECT_EQ(o1, false);
        EXPECT_EQ(d2, true);
        EXPECT_EQ(o2, false);
        EXPECT_EQ(p1, true);
        EXPECT_EQ(p2, true);
    }

    {
        auto int_result = std::is_trivially_copyable<int>::value;
        auto struct_result = std::is_trivially_copyable<Struct>::value;
        auto structx_result = std::is_trivially_copyable<StructX>::value;
        auto s0_result = std::is_trivially_copyable<decltype(s0)>::value;
        auto s5_result = std::is_trivially_copyable<decltype(s5)>::value;
        auto pair_result = std::is_trivially_copyable<std::pair<int, Struct>>::value;
        auto vector_result = std::is_trivially_copyable<std::vector<int>>::value;
        auto d1 = std::is_trivially_copyable<DefaultCopyConstruct>::value;
        auto o1 = std::is_trivially_copyable<OverloadedCopyConstruct>::value;
        auto d2 = std::is_trivially_copyable<DefaultCopyAssignment>::value;
        auto o2 = std::is_trivially_copyable<OverloadedCopyAssignment>::value;
        auto p1 = std::is_trivially_copyable<int*>::value;
        auto p2 = std::is_trivially_copyable<int[]>::value;

        EXPECT_EQ(int_result, true);
        EXPECT_EQ(struct_result, true);
        EXPECT_EQ(structx_result, false);
        EXPECT_EQ(s0_result, true);
        EXPECT_EQ(s5_result, true);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(vector_result, false);
        EXPECT_EQ(d1, true);
        EXPECT_EQ(o1, false);
        EXPECT_EQ(d2, true);
        EXPECT_EQ(o2, false);
        EXPECT_EQ(p1, true); // <<< !important
        EXPECT_EQ(p2, true); // <<< !important
    }

    {
        auto int_result = std::is_trivially_copy_constructible<int>::value;
        auto struct_result = std::is_trivially_copy_constructible<Struct>::value;
        auto structx_result = std::is_trivially_copy_constructible<StructX>::value;
        auto s0_result = std::is_trivially_copy_constructible<decltype(s0)>::value;
        auto s5_result = std::is_trivially_copy_constructible<decltype(s5)>::value;
        auto pair_result =
            std::is_trivially_copy_constructible<std::pair<int, Struct>>::value;
        auto vector_result =
            std::is_trivially_copy_constructible<std::vector<int>>::value;
        auto d1 = std::is_trivially_copy_constructible<DefaultCopyConstruct>::value;
        auto o1 = std::is_trivially_copy_constructible<OverloadedCopyConstruct>::value;
        auto d2 = std::is_trivially_copy_constructible<DefaultCopyAssignment>::value;
        auto o2 = std::is_trivially_copy_constructible<OverloadedCopyAssignment>::value;
        auto p1 = std::is_trivially_copy_constructible<int*>::value;
        auto p2 = std::is_trivially_copy_constructible<int[]>::value;

        EXPECT_EQ(int_result, true);
        EXPECT_EQ(struct_result, true);
        EXPECT_EQ(structx_result, false);
        EXPECT_EQ(s0_result, false);
        EXPECT_EQ(s5_result, false);
        EXPECT_EQ(pair_result, true);
        EXPECT_EQ(vector_result, false);
        EXPECT_EQ(d1, true);
        EXPECT_EQ(o1, false);
        EXPECT_EQ(d2, true);
        EXPECT_EQ(o2, true);
        EXPECT_EQ(p1, true); // <<< !important
        EXPECT_EQ(p2, false); // <<< !important
    }

    {
        auto int_result = std::is_trivially_copy_assignable<int>::value;
        auto struct_result = std::is_trivially_copy_assignable<Struct>::value;
        auto structx_result = std::is_trivially_copy_assignable<StructX>::value;
        auto s0_result = std::is_trivially_copy_assignable<decltype(s0)>::value;
        auto s5_result = std::is_trivially_copy_assignable<decltype(s5)>::value;
        auto pair_result =
            std::is_trivially_copy_assignable<std::pair<int, Struct>>::value;
        auto vector_result = std::is_trivially_copy_assignable<std::vector<int>>::value;
        auto d1 = std::is_trivially_copy_assignable<DefaultCopyConstruct>::value;
        auto o1 = std::is_trivially_copy_assignable<OverloadedCopyConstruct>::value;
        auto d2 = std::is_trivially_copy_assignable<DefaultCopyAssignment>::value;
        auto o2 = std::is_trivially_copy_assignable<OverloadedCopyAssignment>::value;
        auto p1 = std::is_trivially_copy_assignable<int*>::value;
        auto p2 = std::is_trivially_copy_assignable<int[]>::value;

        EXPECT_EQ(int_result, true);
        EXPECT_EQ(struct_result, true);
        EXPECT_EQ(structx_result, false);
        EXPECT_EQ(s0_result, false);
        EXPECT_EQ(s5_result, false);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(vector_result, false);
        EXPECT_EQ(d1, true);
        EXPECT_EQ(o1, true);
        EXPECT_EQ(d2, true);
        EXPECT_EQ(o2, false);
        EXPECT_EQ(p1, true); // <<< !important
        EXPECT_EQ(p2, false); // <<< !important
    }
}

TEST(Traits, IsPointer)
{
    auto int_result = std::is_pointer<int>::value;
    auto pint_result = std::is_pointer<int*>::value;
    auto shared_result = std::is_pointer<std::shared_ptr<int>>::value;
    auto p1 = std::is_pointer<int*>::value;
    auto p2 = std::is_pointer<int[]>::value;
    auto a2 = std::is_array<int[]>::value;

    LOG() << "int*: " << typeid(int*).name() << ", " << sizeof(int*) << std::endl;
    LOG() << "int[]: " << typeid(int[]).name() /*<< ", " << sizeof(int[]) */<< std::endl;
    LOG() << "int[5]: " << typeid(int[5]).name() << ", " << sizeof(int[5]) << std::endl;

    EXPECT_EQ(int_result, false);
    EXPECT_EQ(pint_result, true);
    EXPECT_EQ(shared_result, false);
    EXPECT_EQ(p1, true);
    EXPECT_EQ(p2, false);
    EXPECT_EQ(a2, true);
}

TEST(Traits, IsFunction)
{
    struct Struct
    {
        static void static_method();
    };

    using free_type = decltype(free);
    using bond_type = decltype(&std::vector<int>::size);
    using func_type = remove_class_pointer_t<bond_type>;
    using static_type = decltype(Struct::static_method);

    auto free_result = std::is_function<free_type>::value;
    auto bond_result = std::is_function<bond_type>::value;
    auto func_result = std::is_function<func_type>::value;
    auto functor_reuslt = std::is_function<std::less<int>>::value;
    auto static_reuslt = std::is_function<static_type>::value;

    EXPECT_EQ(free_result, true);
    EXPECT_EQ(bond_result, false);
    EXPECT_EQ(func_result, true);
    EXPECT_EQ(functor_reuslt, false);
    EXPECT_EQ(static_reuslt, true);
}

TEST(Traits, IsClass)
{
    auto int_result = std::is_class<int>::value;
    auto stack_result = std::is_class<std::stack<int>>::value;
    auto functor_reuslt = std::is_class<std::less<int>>::value;

    EXPECT_EQ(int_result, false);
    EXPECT_EQ(stack_result, true);
    EXPECT_EQ(functor_reuslt, true);
}

TEST(Traits, IsObject)
{
    using free_type = decltype(free);
    using bond_type = decltype(&std::vector<int>::size);
    using func_type = remove_class_pointer_t<bond_type>;

    auto free_result = std::is_object<free_type>::value;
    auto bond_result = std::is_object<bond_type>::value;
    auto func_result = std::is_object<func_type>::value;
    auto functor_reuslt = std::is_object<std::less<int>>::value;

    EXPECT_EQ(free_result, false);
    EXPECT_EQ(bond_result, true);
    EXPECT_EQ(func_result, false);
    EXPECT_EQ(functor_reuslt, true);
}

/*
TEST(Traits, is_aggregate)
{
    auto int_result = std::is_aggregate<int>::value;
    auto pair_result = std::is_aggregate<std::pair<int, float>>::value;
    auto vector_result = std::is_aggregate<std::vector<int>>::value;

    EXPECT_EQ(int_result, true);
    EXPECT_EQ(pair_result, true);
    EXPECT_EQ(vector_result, false);
}
*/

TEST(Type, ZeroSize)
{
    auto void_result = is_zero_size_object<void_type>::value;
    auto zero_result = is_zero_size_object<zero_type>::value;
    auto trivial_tresult = is_zero_size_object<trivial_type>::value;
    auto pointer_result = is_zero_size_object<pointer_type>::value;
    auto enum_result = is_zero_size_object<enum_type>::value;
    auto union_result = is_zero_size_object<union_type>::value;
//    auto ad_result = is_zero_size_object<int[]>::value;
    auto af_result = is_zero_size_object<int[5]>::value;

    EXPECT_EQ(void_result, false);
    EXPECT_EQ(zero_result, true);
    EXPECT_EQ(trivial_tresult, false);
    EXPECT_EQ(pointer_result, false);
    EXPECT_EQ(enum_result, false);
    EXPECT_EQ(union_result, false);
//    EXPECT_EQ(ad_result, true);
    EXPECT_EQ(af_result, false);
}

TEST(Type, ReleativeSize)
{
    using TO = uint16_t[3];

    {
        auto result = relative_size_of<TO, uint8_t>::value;

        EXPECT_EQ(result, 6);
    }

    {
        auto result = relative_size_of<TO, uint32_t>::value;

        EXPECT_EQ(result, 2);
    }
}

TEST(Type, ReleativeAligned)
{
    using TO = uint16_t[3];

    {
        auto result = is_relative_aligned<TO, uint8_t>::value;

        EXPECT_EQ(result, true);
    }

    {
        auto result = is_relative_aligned<TO, uint32_t>::value;

        EXPECT_EQ(result, false);
    }
}

} // namespace NAMESPACE

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
