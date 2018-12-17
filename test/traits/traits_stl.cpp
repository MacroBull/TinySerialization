/*

Copyleft 2018 Macrobull

*/

#include <array>
#include <atomic>
#include <chrono>
#include <complex>
#include <forward_list>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <valarray>
#include <vector>

#include <gtest/gtest.h>

#include "serialization/traits_stl.h"
#include "serialization/traits_stl_adaptor.h"
#include "serialization/traits_stl_initializer_list.h"
#include "serialization/traits_stl_ios.h"
#include "serialization/traits_stl_valarray.h"

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

using void_type = void;
using zero_type = char[0];
using trivial_type = bool;
using pointer_type = char*;
using enum_type = Enum;
using union_type = Union;
using function_type = size_t(int*, float);
using functor_type = std::less<int>;
using pair_type = std::pair<char, double>;
using aligned_pair_type = std::pair<int, int>;
using tuple_type = std::tuple<int, char, std::string>;
using vector_type = std::vector<float>;
using allocator_type = vector_type::allocator_type;
using list_type = std::list<float>;
using forward_list_type = std::forward_list<float>;
using string_type = std::string;
using map_type = std::map<int, float>;
using ummap_type = std::unordered_multimap<int, std::string>;
using set_type = std::set<float>;
using queue_type = std::queue<std::pair<int, int>>;
using stack_type = std::stack<float>;
using narray_type = int[];
using array_type = int[9];
//using variant_type = std::variant;
//using optional_type = std::optional;
//using any_type = std::any;

TEST(TraitsPod, SerializationCopyable)
{
    auto void_result = is_serialization_copyable<void_type>::value;
    auto zero_result = is_serialization_copyable<zero_type>::value;
    auto array_result = is_serialization_copyable<array_type>::value;
    auto trivial_tresult = is_serialization_copyable<trivial_type>::value;
    auto pointer_result = is_serialization_copyable<pointer_type>::value;
    auto enum_result = is_serialization_copyable<enum_type>::value;
    auto union_result = is_serialization_copyable<union_type>::value;

    EXPECT_EQ(void_result, false);
    EXPECT_EQ(zero_result, false);
    EXPECT_EQ(array_result, true);
    EXPECT_EQ(trivial_tresult, true);
    EXPECT_EQ(pointer_result, false);
    EXPECT_EQ(enum_result, true);
    EXPECT_EQ(union_result, true);
}

TEST(TraitsPod, Serializable)
{
    {
        auto void_result = is_serializable<void_type>::value;
        auto zero_result = is_serializable<zero_type>::value;
        auto array_result = is_serializable<array_type>::value;
        auto trivial_tresult = is_serializable<trivial_type>::value;
        auto pointer_result = is_serializable<pointer_type>::value;
        auto enum_result = is_serializable<enum_type>::value;
        auto union_result = is_serializable<union_type>::value;
        auto initializer_list_result =
            is_serializable<std::initializer_list<std::string>>::value;

        EXPECT_EQ(void_result, false);
        EXPECT_EQ(zero_result, false);
        EXPECT_EQ(array_result, true);
        EXPECT_EQ(trivial_tresult, true);
        EXPECT_EQ(pointer_result, false);
        EXPECT_EQ(enum_result, true);
        EXPECT_EQ(union_result, true);
        EXPECT_EQ(initializer_list_result, false);
    }
}

TEST(TraitsStlContainer, ContainerType)
{
    {
        auto void_result = is_container_type<void_type>::value;
        auto zero_result = is_container_type<zero_type>::value;
        auto trivial_tresult = is_container_type<trivial_type>::value;
        auto pointer_result = is_container_type<pointer_type>::value;
        auto enum_result = is_container_type<enum_type>::value;
        auto union_result = is_container_type<union_type>::value;
        auto function_result = is_container_type<function_type>::value;
        auto functor_result = is_container_type<functor_type>::value;
        auto pair_result = is_container_type<pair_type>::value;
        auto aligned_pair_result = is_container_type<aligned_pair_type>::value;
        auto tuple_result = is_container_type<tuple_type>::value;
        auto vector_result = is_container_type<vector_type>::value;
        auto allocator_result = is_container_type<allocator_type>::value;
        auto list_result = is_container_type<list_type>::value;
        auto forward_list_result = is_container_type<forward_list_type>::value;
        auto string_result = is_container_type<string_type>::value;
        auto map_result = is_container_type<map_type>::value;
        auto ummap_result = is_container_type<ummap_type>::value;
        auto set_result = is_container_type<set_type>::value;
        auto queue_result = is_container_type<queue_type>::value;

        EXPECT_EQ(void_result, false);
        EXPECT_EQ(zero_result, false);
        EXPECT_EQ(trivial_tresult, false);
        EXPECT_EQ(pointer_result, false);
        EXPECT_EQ(enum_result, false);
        EXPECT_EQ(union_result, false);
        EXPECT_EQ(function_result, false);
        EXPECT_EQ(functor_result, false);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(aligned_pair_result, false);
        EXPECT_EQ(tuple_result, false);
        EXPECT_EQ(vector_result, true);
        EXPECT_EQ(allocator_result, false);
        EXPECT_EQ(list_result, true);
        EXPECT_EQ(forward_list_result, true);
        EXPECT_EQ(string_result, true);
        EXPECT_EQ(map_result, true);
        EXPECT_EQ(ummap_result, true);
        EXPECT_EQ(set_result, true);
        EXPECT_EQ(queue_result, false);
    }
}

TEST(TraitsStlContainer, ContainerTypeOfConst)
{
    {
        auto void_result = is_container_type<const void_type>::value;
        auto zero_result = is_container_type<const zero_type>::value;
        auto trivial_tresult = is_container_type<const trivial_type>::value;
        auto pointer_result = is_container_type<const pointer_type>::value;
        auto enum_result = is_container_type<const enum_type>::value;
        auto union_result = is_container_type<const union_type>::value;
        auto functor_result = is_container_type<const functor_type>::value;
        auto pair_result = is_container_type<const pair_type>::value;
        auto aligned_pair_result = is_container_type<const aligned_pair_type>::value;
        auto tuple_result = is_container_type<const tuple_type>::value;
        auto vector_result = is_container_type<const vector_type>::value;
        auto allocator_result = is_container_type<const allocator_type>::value;
        auto list_result = is_container_type<const list_type>::value;
        auto forward_list_result = is_container_type<const forward_list_type>::value;
        auto string_result = is_container_type<const string_type>::value;
        auto map_result = is_container_type<const map_type>::value;
        auto ummap_result = is_container_type<const ummap_type>::value;
        auto set_result = is_container_type<const set_type>::value;
        auto queue_result = is_container_type<const queue_type>::value;

        EXPECT_EQ(void_result, false);
        EXPECT_EQ(zero_result, false);
        EXPECT_EQ(trivial_tresult, false);
        EXPECT_EQ(pointer_result, false);
        EXPECT_EQ(enum_result, false);
        EXPECT_EQ(union_result, false);
        EXPECT_EQ(functor_result, false);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(aligned_pair_result, false);
        EXPECT_EQ(tuple_result, false);
        EXPECT_EQ(vector_result, true);
        EXPECT_EQ(allocator_result, false);
        EXPECT_EQ(list_result, true);
        EXPECT_EQ(forward_list_result, true);
        EXPECT_EQ(string_result, true);
        EXPECT_EQ(map_result, true);
        EXPECT_EQ(ummap_result, true);
        EXPECT_EQ(set_result, true);
        EXPECT_EQ(queue_result, false);
    }
}

TEST(TraitsStlContainer, BatchInsertable)
{
    {
        auto void_result = is_container_batch_insertable<void_type>::value;
        auto zero_result = is_container_batch_insertable<zero_type>::value;
        auto trivial_tresult = is_container_batch_insertable<trivial_type>::value;
        auto pointer_result = is_container_batch_insertable<pointer_type>::value;
        auto enum_result = is_container_batch_insertable<enum_type>::value;
        auto union_result = is_container_batch_insertable<union_type>::value;
        auto function_result = is_container_batch_insertable<function_type>::value;
        auto functor_result = is_container_batch_insertable<functor_type>::value;
        auto pair_result = is_container_batch_insertable<pair_type>::value;
        auto aligned_pair_result = is_container_batch_insertable<aligned_pair_type>::value;
        auto tuple_result = is_container_batch_insertable<tuple_type>::value;
        auto vector_result = is_container_batch_insertable<vector_type>::value;
        auto allocator_result = is_container_batch_insertable<allocator_type>::value;
        auto list_result = is_container_batch_insertable<list_type>::value;
        auto forward_list_result = is_container_batch_insertable<forward_list_type>::value;
        auto string_result = is_container_batch_insertable<string_type>::value;
        auto map_result = is_container_batch_insertable<map_type>::value;
        auto ummap_result = is_container_batch_insertable<ummap_type>::value;
        auto set_result = is_container_batch_insertable<set_type>::value;
        auto queue_result = is_container_batch_insertable<queue_type>::value;
        auto stack_result = is_container_batch_insertable<stack_type>::value;

        EXPECT_EQ(void_result, false);
        EXPECT_EQ(zero_result, false);
        EXPECT_EQ(trivial_tresult, false);
        EXPECT_EQ(pointer_result, false);
        EXPECT_EQ(enum_result, false);
        EXPECT_EQ(union_result, false);
        EXPECT_EQ(function_result, false);
        EXPECT_EQ(functor_result, false);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(aligned_pair_result, false);
        EXPECT_EQ(tuple_result, false);
        EXPECT_EQ(vector_result, false);
        EXPECT_EQ(allocator_result, false);
        EXPECT_EQ(list_result, false);
        EXPECT_EQ(forward_list_result, false);
        EXPECT_EQ(string_result, false);
        EXPECT_EQ(map_result, true);
        EXPECT_EQ(ummap_result, false);
        EXPECT_EQ(set_result, true);
        EXPECT_EQ(queue_result, false);
        EXPECT_EQ(stack_result, false);
    }
}

TEST(TraitsStlContainer, SerializationCopyable)
{
    using Type = std::vector<int>;

    auto result = is_serialization_copyable<Type>::value;

    EXPECT_EQ(result, false);
}

TEST(TraitsStlContainer, Serializable)
{
    {
        using Type = std::vector<int>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }

    {
        using Type = const std::vector<int>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }

    {
        using Type = std::vector<int*>&;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
    }

    {
        using Type = std::string;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }

    {
        // value_type is <const int, std::string>
        using Type = std::map<int, std::string>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }

    {
        using Type = std::map<const int, std::string>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }
}

/*
TEST(TraitsStlSequential, SequentialType)
{
    {
        auto void_result = is_sequential_type_with_get_iterator<void_type>::value;
        auto zero_result = is_sequential_type_with_get_iterator<zero_type>::value;
        auto trivial_tresult = is_sequential_type_with_get_iterator<trivial_type>::value;
        auto pointer_result = is_sequential_type_with_get_iterator<pointer_type>::value;
        auto enum_result = is_sequential_type_with_get_iterator<enum_type>::value;
        auto union_result = is_sequential_type_with_get_iterator<union_type>::value;
        auto function_result = is_sequential_type_with_get_iterator<function_type>::value;
        auto functor_result = is_sequential_type_with_get_iterator<functor_type>::value;
        auto pair_result = is_sequential_type_with_get_iterator<pair_type>::value;
        auto aligned_pair_result = is_sequential_type_with_get_iterator<aligned_pair_type>::value;
        auto tuple_result = is_sequential_type_with_get_iterator<tuple_type>::value;
        auto vector_result = is_sequential_type_with_get_iterator<vector_type>::value;
        auto allocator_result = is_sequential_type_with_get_iterator<allocator_type>::value;
        auto list_result = is_sequential_type_with_get_iterator<list_type>::value;
        auto forward_list_result = is_sequential_type_with_get_iterator<forward_list_type>::value;
        auto string_result = is_sequential_type_with_get_iterator<string_type>::value;
        auto map_result = is_sequential_type_with_get_iterator<map_type>::value;
        auto ummap_result = is_sequential_type_with_get_iterator<ummap_type>::value;
        auto set_result = is_sequential_type_with_get_iterator<set_type>::value;
        auto queue_result = is_sequential_type_with_get_iterator<queue_type>::value;

        EXPECT_EQ(void_result, false);
        EXPECT_EQ(zero_result, false);
        EXPECT_EQ(trivial_tresult, false);
        EXPECT_EQ(pointer_result, false);
        EXPECT_EQ(enum_result, false);
        EXPECT_EQ(union_result, false);
        EXPECT_EQ(function_result, false);
        EXPECT_EQ(functor_result, false);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(aligned_pair_result, false);
        EXPECT_EQ(tuple_result, false);
        EXPECT_EQ(vector_result, true);
        EXPECT_EQ(allocator_result, false);
        EXPECT_EQ(list_result, true);
        EXPECT_EQ(forward_list_result, true);
        EXPECT_EQ(string_result, true);
        EXPECT_EQ(map_result, false);
        EXPECT_EQ(ummap_result, false);
        EXPECT_EQ(set_result, true);
        EXPECT_EQ(queue_result, false);
    }

    {
        auto void_result = is_sequential_type_with_set_iterator<void_type>::value;
        auto zero_result = is_sequential_type_with_set_iterator<zero_type>::value;
        auto trivial_tresult = is_sequential_type_with_set_iterator<trivial_type>::value;
        auto pointer_result = is_sequential_type_with_set_iterator<pointer_type>::value;
        auto enum_result = is_sequential_type_with_set_iterator<enum_type>::value;
        auto union_result = is_sequential_type_with_set_iterator<union_type>::value;
        auto function_result = is_sequential_type_with_set_iterator<function_type>::value;
        auto functor_result = is_sequential_type_with_set_iterator<functor_type>::value;
        auto pair_result = is_sequential_type_with_set_iterator<pair_type>::value;
        auto aligned_pair_result = is_sequential_type_with_set_iterator<aligned_pair_type>::value;
        auto tuple_result = is_sequential_type_with_set_iterator<tuple_type>::value;
        auto vector_result = is_sequential_type_with_set_iterator<vector_type>::value;
        auto allocator_result = is_sequential_type_with_set_iterator<allocator_type>::value;
        auto list_result = is_sequential_type_with_set_iterator<list_type>::value;
        auto forward_list_result = is_sequential_type_with_set_iterator<forward_list_type>::value;
        auto string_result = is_sequential_type_with_set_iterator<string_type>::value;
        auto map_result = is_sequential_type_with_set_iterator<map_type>::value;
        auto ummap_result = is_sequential_type_with_set_iterator<ummap_type>::value;
        auto set_result = is_sequential_type_with_set_iterator<set_type>::value;
        auto queue_result = is_sequential_type_with_set_iterator<queue_type>::value;

        EXPECT_EQ(void_result, false);
        EXPECT_EQ(zero_result, false);
        EXPECT_EQ(trivial_tresult, false);
        EXPECT_EQ(pointer_result, false);
        EXPECT_EQ(enum_result, false);
        EXPECT_EQ(union_result, false);
        EXPECT_EQ(function_result, false);
        EXPECT_EQ(functor_result, false);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(aligned_pair_result, false);
        EXPECT_EQ(tuple_result, false);
        EXPECT_EQ(vector_result, true);
        EXPECT_EQ(allocator_result, false);
        EXPECT_EQ(list_result, true);
        EXPECT_EQ(forward_list_result, true);
        EXPECT_EQ(string_result, true);
        EXPECT_EQ(map_result, false);
        EXPECT_EQ(ummap_result, false);
        EXPECT_EQ(set_result, true);
        EXPECT_EQ(queue_result, false);
    }

    {
        auto void_result = is_sequential_type<void_type>::value;
        auto zero_result = is_sequential_type<zero_type>::value;
        auto trivial_tresult = is_sequential_type<trivial_type>::value;
        auto pointer_result = is_sequential_type<pointer_type>::value;
        auto enum_result = is_sequential_type<enum_type>::value;
        auto union_result = is_sequential_type<union_type>::value;
        auto function_result = is_sequential_type<function_type>::value;
        auto functor_result = is_sequential_type<functor_type>::value;
        auto pair_result = is_sequential_type<pair_type>::value;
        auto aligned_pair_result = is_sequential_type<aligned_pair_type>::value;
        auto tuple_result = is_sequential_type<tuple_type>::value;
        auto vector_result = is_sequential_type<vector_type>::value;
        auto allocator_result = is_sequential_type<allocator_type>::value;
        auto list_result = is_sequential_type<list_type>::value;
        auto forward_list_result = is_sequential_type<forward_list_type>::value;
        auto string_result = is_sequential_type<string_type>::value;
        auto map_result = is_sequential_type<map_type>::value;
        auto ummap_result = is_sequential_type<ummap_type>::value;
        auto set_result = is_sequential_type<set_type>::value;
        auto queue_result = is_sequential_type<queue_type>::value;

        EXPECT_EQ(void_result, false);
        EXPECT_EQ(zero_result, false);
        EXPECT_EQ(trivial_tresult, false);
        EXPECT_EQ(pointer_result, false);
        EXPECT_EQ(enum_result, false);
        EXPECT_EQ(union_result, false);
        EXPECT_EQ(function_result, false);
        EXPECT_EQ(functor_result, false);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(aligned_pair_result, false);
        EXPECT_EQ(tuple_result, false);
        EXPECT_EQ(vector_result, true);
        EXPECT_EQ(allocator_result, false);
        EXPECT_EQ(list_result, true);
        EXPECT_EQ(forward_list_result, true);
        EXPECT_EQ(string_result, true);
        EXPECT_EQ(map_result, false);
        EXPECT_EQ(ummap_result, false);
        EXPECT_EQ(set_result, true);
        EXPECT_EQ(queue_result, false);
    }
}

TEST(TraitsStlAssociative, AssociativeType)
{
    {
        auto void_result = is_associative_type_with_get_iterator<void_type>::value;
        auto zero_result = is_associative_type_with_get_iterator<zero_type>::value;
        auto trivial_tresult = is_associative_type_with_get_iterator<trivial_type>::value;
        auto pointer_result = is_associative_type_with_get_iterator<pointer_type>::value;
        auto enum_result = is_associative_type_with_get_iterator<enum_type>::value;
        auto union_result = is_associative_type_with_get_iterator<union_type>::value;
        auto function_result = is_associative_type_with_get_iterator<function_type>::value;
        auto functor_result = is_associative_type_with_get_iterator<functor_type>::value;
        auto pair_result = is_associative_type_with_get_iterator<pair_type>::value;
        auto aligned_pair_result = is_associative_type_with_get_iterator<aligned_pair_type>::value;
        auto tuple_result = is_associative_type_with_get_iterator<tuple_type>::value;
        auto vector_result = is_associative_type_with_get_iterator<vector_type>::value;
        auto allocator_result = is_associative_type_with_get_iterator<allocator_type>::value;
        auto list_result = is_associative_type_with_get_iterator<list_type>::value;
        auto forward_list_result = is_associative_type_with_get_iterator<forward_list_type>::value;
        auto string_result = is_associative_type_with_get_iterator<string_type>::value;
        auto map_result = is_associative_type_with_get_iterator<map_type>::value;
        auto ummap_result = is_associative_type_with_get_iterator<ummap_type>::value;
        auto set_result = is_associative_type_with_get_iterator<set_type>::value;
        auto queue_result = is_associative_type_with_get_iterator<queue_type>::value;

        EXPECT_EQ(void_result, false);
        EXPECT_EQ(zero_result, false);
        EXPECT_EQ(trivial_tresult, false);
        EXPECT_EQ(pointer_result, false);
        EXPECT_EQ(enum_result, false);
        EXPECT_EQ(union_result, false);
        EXPECT_EQ(function_result, false);
        EXPECT_EQ(functor_result, false);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(aligned_pair_result, false);
        EXPECT_EQ(tuple_result, false);
        EXPECT_EQ(vector_result, false);
        EXPECT_EQ(allocator_result, false);
        EXPECT_EQ(list_result, false);
        EXPECT_EQ(forward_list_result, false);
        EXPECT_EQ(string_result, false);
        EXPECT_EQ(map_result, true);
        EXPECT_EQ(ummap_result, true);
        EXPECT_EQ(set_result, false);
        EXPECT_EQ(queue_result, false);
    }

    {
        auto void_result = is_associative_type_with_set_iterator<void_type>::value;
        auto zero_result = is_associative_type_with_set_iterator<zero_type>::value;
        auto trivial_tresult = is_associative_type_with_set_iterator<trivial_type>::value;
        auto pointer_result = is_associative_type_with_set_iterator<pointer_type>::value;
        auto enum_result = is_associative_type_with_set_iterator<enum_type>::value;
        auto union_result = is_associative_type_with_set_iterator<union_type>::value;
        auto function_result = is_associative_type_with_set_iterator<function_type>::value;
        auto functor_result = is_associative_type_with_set_iterator<functor_type>::value;
        auto pair_result = is_associative_type_with_set_iterator<pair_type>::value;
        auto aligned_pair_result = is_associative_type_with_set_iterator<aligned_pair_type>::value;
        auto tuple_result = is_associative_type_with_set_iterator<tuple_type>::value;
        auto vector_result = is_associative_type_with_set_iterator<vector_type>::value;
        auto allocator_result = is_associative_type_with_set_iterator<allocator_type>::value;
        auto list_result = is_associative_type_with_set_iterator<list_type>::value;
        auto forward_list_result = is_associative_type_with_set_iterator<forward_list_type>::value;
        auto string_result = is_associative_type_with_set_iterator<string_type>::value;
        auto map_result = is_associative_type_with_set_iterator<map_type>::value;
        auto ummap_result = is_associative_type_with_set_iterator<ummap_type>::value;
        auto set_result = is_associative_type_with_set_iterator<set_type>::value;
        auto queue_result = is_associative_type_with_set_iterator<queue_type>::value;

        EXPECT_EQ(void_result, false);
        EXPECT_EQ(zero_result, false);
        EXPECT_EQ(trivial_tresult, false);
        EXPECT_EQ(pointer_result, false);
        EXPECT_EQ(enum_result, false);
        EXPECT_EQ(union_result, false);
        EXPECT_EQ(function_result, false);
        EXPECT_EQ(functor_result, false);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(aligned_pair_result, false);
        EXPECT_EQ(tuple_result, false);
        EXPECT_EQ(vector_result, false);
        EXPECT_EQ(allocator_result, false);
        EXPECT_EQ(list_result, false);
        EXPECT_EQ(forward_list_result, false);
        EXPECT_EQ(string_result, false);
        EXPECT_EQ(map_result, true);
        EXPECT_EQ(ummap_result, true);
        EXPECT_EQ(set_result, false);
        EXPECT_EQ(queue_result, false);
    }

    {
        auto void_result = is_associative_type<void_type>::value;
        auto zero_result = is_associative_type<zero_type>::value;
        auto trivial_tresult = is_associative_type<trivial_type>::value;
        auto pointer_result = is_associative_type<pointer_type>::value;
        auto enum_result = is_associative_type<enum_type>::value;
        auto union_result = is_associative_type<union_type>::value;
        auto function_result = is_associative_type<function_type>::value;
        auto functor_result = is_associative_type<functor_type>::value;
        auto pair_result = is_associative_type<pair_type>::value;
        auto aligned_pair_result = is_associative_type<aligned_pair_type>::value;
        auto tuple_result = is_associative_type<tuple_type>::value;
        auto vector_result = is_associative_type<vector_type>::value;
        auto allocator_result = is_associative_type<allocator_type>::value;
        auto list_result = is_associative_type<list_type>::value;
        auto forward_list_result = is_associative_type<forward_list_type>::value;
        auto string_result = is_associative_type<string_type>::value;
        auto map_result = is_associative_type<map_type>::value;
        auto ummap_result = is_associative_type<ummap_type>::value;
        auto set_result = is_associative_type<set_type>::value;
        auto queue_result = is_associative_type<queue_type>::value;

        EXPECT_EQ(void_result, false);
        EXPECT_EQ(zero_result, false);
        EXPECT_EQ(trivial_tresult, false);
        EXPECT_EQ(pointer_result, false);
        EXPECT_EQ(enum_result, false);
        EXPECT_EQ(union_result, false);
        EXPECT_EQ(function_result, false);
        EXPECT_EQ(functor_result, false);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(aligned_pair_result, false);
        EXPECT_EQ(tuple_result, false);
        EXPECT_EQ(vector_result, false);
        EXPECT_EQ(allocator_result, false);
        EXPECT_EQ(list_result, false);
        EXPECT_EQ(forward_list_result, false);
        EXPECT_EQ(string_result, false);
        EXPECT_EQ(map_result, true);
        EXPECT_EQ(ummap_result, true);
        EXPECT_EQ(set_result, false);
        EXPECT_EQ(queue_result, false);
    }
}
*/

TEST(TraitsStlAdaptor, AdaptorType)
{
    {
        auto void_result = is_adaptor_type<void_type>::value;
        auto zero_result = is_adaptor_type<zero_type>::value;
        auto trivial_tresult = is_adaptor_type<trivial_type>::value;
        auto pointer_result = is_adaptor_type<pointer_type>::value;
        auto enum_result = is_adaptor_type<enum_type>::value;
        auto union_result = is_adaptor_type<union_type>::value;
        auto function_result = is_adaptor_type<function_type>::value;
        auto functor_result = is_adaptor_type<functor_type>::value;
        auto pair_result = is_adaptor_type<pair_type>::value;
        auto aligned_pair_result = is_adaptor_type<aligned_pair_type>::value;
        auto tuple_result = is_adaptor_type<tuple_type>::value;
        auto vector_result = is_adaptor_type<vector_type>::value;
        auto allocator_result = is_adaptor_type<allocator_type>::value;
        auto list_result = is_adaptor_type<list_type>::value;
        auto forward_list_result = is_adaptor_type<forward_list_type>::value;
        auto string_result = is_adaptor_type<string_type>::value;
        auto map_result = is_adaptor_type<map_type>::value;
        auto ummap_result = is_adaptor_type<ummap_type>::value;
        auto set_result = is_adaptor_type<set_type>::value;
        auto queue_result = is_adaptor_type<queue_type>::value;
        auto stack_result = is_adaptor_type<stack_type>::value;

        EXPECT_EQ(void_result, false);
        EXPECT_EQ(zero_result, false);
        EXPECT_EQ(trivial_tresult, false);
        EXPECT_EQ(pointer_result, false);
        EXPECT_EQ(enum_result, false);
        EXPECT_EQ(union_result, false);
        EXPECT_EQ(function_result, false);
        EXPECT_EQ(functor_result, false);
        EXPECT_EQ(pair_result, false);
        EXPECT_EQ(aligned_pair_result, false);
        EXPECT_EQ(tuple_result, false);
        EXPECT_EQ(vector_result, false);
        EXPECT_EQ(allocator_result, false);
        EXPECT_EQ(list_result, false);
        EXPECT_EQ(forward_list_result, false);
        EXPECT_EQ(string_result, false);
        EXPECT_EQ(map_result, false);
        EXPECT_EQ(ummap_result, false);
        EXPECT_EQ(set_result, false);
        EXPECT_EQ(queue_result, true);
        EXPECT_EQ(stack_result, true);
    }
}

TEST(TraitsStlAdaptor, SerializationCopyable)
{
    auto queue_result = is_serialization_copyable<queue_type>::value;
    auto stack_result = is_serialization_copyable<stack_type>::value;
    auto pq_result = is_serialization_copyable<std::priority_queue<int>>::value;

    EXPECT_EQ(queue_result, false);
    EXPECT_EQ(stack_result, false);
    EXPECT_EQ(pq_result, false);
}

TEST(TraitsStlAdaptor, Serializable)
{
    auto queue_result = is_serializable<queue_type>::value;
    auto stack_result = is_serializable<stack_type>::value;
    auto pq_result = is_serializable<std::priority_queue<int>>::value;

    EXPECT_EQ(queue_result, true);
    EXPECT_EQ(stack_result, true);
    EXPECT_EQ(pq_result, false);
}

TEST(TraitsStlPair, SerializationCopyable)
{
    {
        using Type = std::pair<int, int>;

        auto result = is_serialization_copyable<Type>::value;

        EXPECT_EQ(result, true);
    }

    {
        using Type = std::pair<char, int>;

        auto result = is_serialization_copyable<Type>::value;

        EXPECT_EQ(result, true); // <<< !important
    }

    {
        using Type = std::pair<char, int*>;

        auto result = is_serialization_copyable<Type>::value;

        EXPECT_EQ(result, false);
    }

    {
        using Type = std::pair<float, std::string>;

        auto result = is_serialization_copyable<Type>::value;

        EXPECT_EQ(result, false);
    }
}

TEST(TraitsStlPair, Serializable)
{
    {
        using Type = std::pair<int, int>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }

    {
        using Type = std::pair<char, int>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }

    {
        using Type = std::pair<char, int*>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
    }

    {
        using Type = std::pair<float, std::string>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }

    {
        using Type = std::pair<const float, std::string>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }

    {
        using Type = const std::pair<float, std::string>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }
}

TEST(TraitsStlTuple, SerializationCopyable)
{
    {
        using Type = std::tuple<int, float>;

        auto result0 = is_serialization_copyable<Type>::value;

        EXPECT_EQ(result0, true);
    }

    {
        using Type = tuple_type;

        auto result0 = is_serialization_copyable<Type>::value;

        EXPECT_EQ(result0, false);
    }
}

TEST(TraitsStlTuple, Serializable)
{
    {
        using Type = std::tuple<int, float>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }

    {
        using Type = tuple_type;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }
}

TEST(TraitsStlAllocator, SerializationCopyable)
{
    {
        using Type = std::allocator<int>;

        auto result0 = is_serialization_copyable<Type>::value;

        EXPECT_EQ(result0, false);
    }
}

TEST(TraitsStlAllocator, Serializable)
{
    {
        using Type = std::allocator<int>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
    }
}

TEST(TraitsStlAtomic, SerializationCopyable)
{
    {
        using Type = std::atomic<int>;

        auto result0 = is_serialization_copyable<Type>::value;

        EXPECT_EQ(result0, true);
    }
}

TEST(TraitsStlAtomic, Serializable)
{
    {
        using Type = std::atomic<int>;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }
}

TEST(TraitsStlIoStream, SerializationCopyable)
{
    {
        using Type = std::stringbuf;

        auto result0 = is_serialization_copyable<Type>::value;

        EXPECT_EQ(result0, false);
    }

    {
        using Type = std::stringstream;

        auto result0 = is_serialization_copyable<Type>::value;

        EXPECT_EQ(result0, false);
    }
}

TEST(TraitsStlIoStream, Serializable)
{
    {
        using Type = std::stringbuf;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }

    {
        using Type = std::stringstream;

        auto result0 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
    }
}

TEST(TraitsStlValArray, ValArray)
{
    {
        using Type = std::valarray<float>;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
        EXPECT_EQ(result1, true);
    }
}

TEST(TraitsStlValArray, Slice)
{
    {
        using Type = std::slice;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
        EXPECT_EQ(result1, true);
    }

    {
        using Type = std::gslice;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
        EXPECT_EQ(result1, true);
    }
}

TEST(TraitsStlValArray, SliceArray)
{
    {
        using Type = std::slice_array<int>;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
        EXPECT_EQ(result1, true);
    }

    {
        using Type = std::gslice_array<int>;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
        EXPECT_EQ(result1, true);
    }

    {
        using Type = std::mask_array<int>;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
        EXPECT_EQ(result1, true);
    }

    {
        using Type = std::indirect_array<int>;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
        EXPECT_EQ(result1, true);
    }
}

TEST(TraitsStlChrono, Duration)
{
    {
        using Type = std::chrono::duration<int, std::ratio<1, 100>>;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
        EXPECT_EQ(result1, true);
    }
}

TEST(TraitsStlChrono, TimePoint)
{
    {
        using Type = std::chrono::system_clock::time_point;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, true);
        EXPECT_EQ(result1, true);
    }
}

TEST(TraitsStlPtr, SharedPtr)
{
    {
        using Type = std::shared_ptr<int>;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
        EXPECT_EQ(result1, false);
    }

    {
        using Type = std::shared_ptr<int[]>;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
        EXPECT_EQ(result1, false);
    }
}

TEST(TraitsStlPtr, UniquePtr)
{
    {
        using Type = std::unique_ptr<int>; //

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
        EXPECT_EQ(result1, true);
    }

    {
        using Type = std::unique_ptr<int[]>;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
        EXPECT_EQ(result1, false);
    }

    {
        using Type = std::unique_ptr<std::string>;

        auto result0 = is_serialization_copyable<Type>::value;
        auto result1 = is_serializable<Type>::value;

        EXPECT_EQ(result0, false);
        EXPECT_EQ(result1, true);
    }
}

} // namespace NAMESPACE

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
