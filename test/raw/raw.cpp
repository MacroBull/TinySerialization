/*

Copyleft 2018 Macrobull

*/

#include <gtest/gtest.h>

#include "test.h"

#include "serialization/raw.h"

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

    const int const_value[3] = { 1, 2, 3 };
    static constexpr int constexpr_value = 42;

    volatile double* volatile_object;
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

TEST(RawSpecial, Void)
{
    using Type = void_type;

    auto result0 = is_serialization_copyable<Type>::value;
    auto result1 = is_serializable<Type>::value;

    EXPECT_EQ(result0, false);
    EXPECT_EQ(result1, false);
}

TEST(RawSpecial, ZeroSize)
{
    using Type = zero_type;

    auto result0 = is_serialization_copyable<Type>::value;
    auto result1 = is_serializable<Type>::value;

    EXPECT_EQ(result0, false);
    EXPECT_EQ(result1, false);
}

TEST(RawSpecial, FlexibleArray)
{
    using Type = int[];

    auto result0 = is_serialization_copyable<Type>::value;
    auto result1 = is_serializable<Type>::value;

    EXPECT_EQ(result0, false);
    EXPECT_EQ(result1, false);
}

TEST(RawSpecial, Array)
{
    using Type = int[5];

    auto result0 = is_serialization_copyable<Type>::value;
    auto result1 = is_serializable<Type>::value;

    EXPECT_EQ(result0, true);
    EXPECT_EQ(result1, true);
}

TEST(RawTrivival, Trivival)
{
    using Type = trivial_type;

    {
        char buf[sizeof(Type)];
        Type ti, to;

        ti = true;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type)];
        const Type ti = true;
        Type to;

        auto pi = serialize(buf, ti);
        const auto cbuf = buf;
        auto po = deserialize(cbuf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type)];
        Type t1 = true, t2;
        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type)];
        Type t1 = true, t2;
        const Type &ti = t1;
        Type &to = t2;

        auto pi = serialize(buf, ti);
        const auto cbuf = buf;
        auto po = deserialize(cbuf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }
}

TEST(RawTrivival, Pointer)
{
    using Type = pointer_type;

    auto can_serialize = is_serializable<Type>::value;

    EXPECT_EQ(can_serialize, false);
}

TEST(RawTrivival, Enum)
{
    using Type = enum_type;

    {
        char buf[sizeof(Type)];
        Type ti, to;

        ti = Enum::Banana;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type)];
        const Type ti = Enum::Banana;
        Type to;

        auto pi = serialize(buf, ti);
        const auto cbuf = buf;
        auto po = deserialize(cbuf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type)];
        Type t1 = Enum::Banana, t2;
        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type)];
        Type t1 = Enum::Banana, t2;
        const Type &ti = t1;
        Type &to = t2;

        auto pi = serialize(buf, ti);
        const auto cbuf = buf;
        auto po = deserialize(cbuf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }
}

TEST(RawTrivival, Union)
{
    using Type = union_type;

    {
        char buf[sizeof(Type)];
        Type ti, to;

        ti.float_value = 3.14f;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.float_value, to.float_value);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type)];
        const Type ti = { .float_value = 3.14f };
        Type to;

        auto pi = serialize(buf, ti);
        const auto cbuf = buf;
        auto po = deserialize(cbuf, to);

        EXPECT_EQ(pi, po);

        EXPECT_EQ(ti.float_value, to.float_value);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type)];
        Type t1 = { .float_value = 3.14f }, t2;
        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);

        EXPECT_EQ(ti.float_value, to.float_value);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type)];
        Type t1 = { .float_value = 3.14f }, t2;
        const Type &ti = t1;
        Type &to = t2;

        auto pi = serialize(buf, ti);
        const auto cbuf = buf;
        auto po = deserialize(cbuf, to);

        EXPECT_EQ(pi, po);

        EXPECT_EQ(ti.float_value, to.float_value);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }
}

TEST(RawTrivival, Struct0)
{
    using Type = Struct;

    auto can_serialize = is_serializable<Type>::value;

    EXPECT_EQ(can_serialize, false);
}

TEST(RawTrivival, Struct1)
{
    using Type = struct {
        char *x; // public pointer
        float y;
    };

    auto can_serialize = is_serializable<Type>::value;

    EXPECT_EQ(can_serialize, false);
}

TEST(RawTrivival, Struct2)
{
    using Type = class {
        char *x; // private pointer
        float y;
    };

    auto can_serialize = is_serializable<Type>::value;

    EXPECT_EQ(can_serialize, false);
}

TEST(RawTrivival, Struct3)
{
    using Type = struct Struct {
        char x;
        float y;

        Struct() {}
    };

    auto can_serialize = is_serializable<Type>::value;

    EXPECT_EQ(can_serialize, true);
}

TEST(RawTrivival, Struct4)
{
    using Type = class Class {
        char x;
        float y;

    public:
        Class(const Class&);
    };

    auto can_serialize = is_serializable<Type>::value;

    EXPECT_EQ(can_serialize, false);
}

TEST(RawTrivival, Struct5)
{
    using Type = struct Struct {
        char x;
        float y;

        Struct& operator=(const Struct&)
        {
            return *this;
        }
    };

    auto can_serialize = is_serializable<Type>::value;

    EXPECT_EQ(can_serialize, false);
}

TEST(RawTrivival, Struct10)
{
    using Type = struct {
        char x;
        float y;
    };

    {
        char buf[sizeof(Type)];
        Type ti, to;

        ti = { 42, 3.14f };

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.x, to.x);
        EXPECT_EQ(ti.y, to.y);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type)];
        const Type ti = { 42, 3.14f };
        Type to;

        auto pi = serialize(buf, ti);
        const auto cbuf = buf;
        auto po = deserialize(cbuf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.x, to.x);
        EXPECT_EQ(ti.y, to.y);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type)];
        Type t1 = { 42, 3.14f }, t2;
        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.x, to.x);
        EXPECT_EQ(ti.y, to.y);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type)];
        Type t1 = { 42, 3.14f }, t2;
        const Type &ti = t1;
        Type &to = t2;

        auto pi = serialize(buf, ti);
        const auto cbuf = buf;
        auto po = deserialize(cbuf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.x, to.x);
        EXPECT_EQ(ti.y, to.y);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }
}

TEST(RawTrivival, Struct11)
{
    float y = 3.14f;

    // NOTE(): reference is pointer, would not match Type2 float y
    using Type1 = struct {
        const char x;
        float &y;
    };

    using Type2 = struct {
        char x;
        size_t y; // <<< as pointer
    };

    // trick here, is_serializable should be false

    {
        char buf[sizeof(Type1)];
        Type1 ti = { 42, y };
        Type2 to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.x, to.x);
        EXPECT_EQ(ti.y, *reinterpret_cast<float*>(to.y));

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type1)];
        const Type1 ti = { 42, y };
        Type2 to;

        auto pi = serialize(buf, ti);
        const auto cbuf = buf;
        auto po = deserialize(cbuf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.x, to.x);
        EXPECT_EQ(ti.y, *reinterpret_cast<float*>(to.y));

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type1)];
        Type1 t = { 42, y };
        Type1 &ti = t, &to = t;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.x, to.x);
        EXPECT_EQ(ti.y, to.y);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[sizeof(Type1)];
        Type1 t = { 42, y } ;
        const Type1 &ti = t;
        Type1 &to = t;

        auto pi = serialize(buf, ti);
        const auto cbuf = buf;
        auto po = deserialize(cbuf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.x, to.x);
        EXPECT_EQ(ti.y, to.y);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }
}

} // namespace NAMESPACE

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
