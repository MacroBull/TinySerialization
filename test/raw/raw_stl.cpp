/*

Copyleft 2018 Macrobull

*/

#include <atomic>
#include <complex>
#include <chrono>
#include <deque>
#include <forward_list>
#include <initializer_list>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <valarray>
#include <vector>

#include <gtest/gtest.h>

#include "test.h"

#include "serialization/raw_stl.h"
#include "serialization/raw_stl_adaptor.h"
#include "serialization/raw_stl_initializer_list.h"
#include "serialization/raw_stl_valarray.h"

namespace NAMESPACE
{

TEST(RawStlAtomic, Atomic)
{
    using ValueType = float;
    using Type = std::atomic<ValueType>;
    const size_t s = sizeof(ValueType);

    LOG() << "stack size of std::atomic<float> is: " <<
             sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti, to;

        ti.store(1.23f);

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::atomic<float> instance is: " <<
                 size << std::endl;
    }
}

TEST(RawStlChrono, Chrono)
{
    using Type =  std::chrono::system_clock::time_point;
    const size_t s = sizeof(Type);

    LOG() << "stack size of std::chrono::system_clock::time_point is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti, to;

        ti = std::chrono::system_clock::now();

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::chrono::system_clock::time_point instance is: " <<
            size << std::endl;
    }
}

TEST(RawStlComplex, Complex)
{
    using ValueType = float;
    using Type = std::complex<ValueType>;
    const size_t s = sizeof(Type);

    LOG() << "stack size of std::atomic<float> is: " <<
             sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti { -2.5, .7f }, to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::atomic<float> instance is: " <<
                 size << std::endl;
    }
}

TEST(RawStlPair, FastCopy)
{
    using Type = std::pair<char, double>;

    LOG() << "stack size of std::pair<char, double> is: " << sizeof(Type) << std::endl;

    {
        char buf[sizeof(Type)];
        Type ti, to;

        ti = { 42, 3.14f };

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::pair<char, double> instance is: " <<
            size << std::endl;
    }

    {
        char buf[sizeof(Type)];
        const Type ti = { 42, 3.14f };
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
        Type t1 = { 42, 3.14f }, t2;
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
        Type t1 = { 42, 3.14f }, t2;
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

TEST(RawStlPair, FastCopy2)
{
    using Type = std::pair<char, std::pair<const int, float>>;

    LOG() << "stack size of std::pair<char, std::pair<const int, float>> is: " <<
        sizeof(Type) << std::endl;

    {
        char buf[sizeof(Type)];
        Type ti, to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() <<
            "serialized size of std::pair<char, std::pair<const int, float>> instance is: " <<
            size << std::endl;
    }

    {
        char buf[sizeof(Type)];
        const Type ti;
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
        Type t1, t2;
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
        Type t1, t2;
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

TEST(RawStlPair, Standard)
{
    using Type = std::pair<int, std::string>;
    const size_t s = sizeof(int) + sizeof(size_t) + 6;

    LOG() << "stack size of std::pair<int, std::string> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti, to = { 233, "6" };

        ti = { 42, "3.1415" };

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::pair<int, std::string> instance is: " <<
            size << std::endl;
    }

    {
        char buf[s];
        const Type ti = { 42, "3.1415" };
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
        char buf[s];
        Type t1 = { 42, "3.1415" }, t2;
        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[s];
        Type t1 = { 42, "3.1415" }, t2;
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

TEST(RawStlTuple, FastCopy)
{
    using Type = std::tuple<char, double, float>;

    LOG() << "stack size of std::tuple<char, double, float> is: " <<
        sizeof(Type) << std::endl;

    {
        char buf[sizeof(Type)];
        Type ti, to;

        ti = std::make_tuple('1', 2., 3.f);

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::tuple<char, double, float> instance is: " <<
            size << std::endl;
    }

    {
        char buf[sizeof(Type)];
        const Type ti = std::make_tuple('1', 2., 3.f);
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
        Type t1 = std::make_tuple('1', 2., 3.f), t2;
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
        Type t1 = std::make_tuple('1', 2., 3.f), t2;
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

TEST(RawStlTuple, Standard)
{
    using Type = std::tuple<int, std::string>;
    const size_t s = sizeof(int) + sizeof(size_t) + 6;

    LOG() << "stack size of std::tuple<int, std::string> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti, to;

        ti = { 42, "3.1415" };

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::tuple<int, std::string> instance is: " <<
            size << std::endl;
    }

    {
        char buf[s];
        const Type ti = { 42, "3.1415" };
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
        char buf[s];
        Type t1 = { 42, "3.1415" }, t2;
        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[s];
        Type t1 = { 42, "3.1415" }, t2;
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

TEST(RawStlContainer, Array)
{
    const size_t n = 9;
    using ValueType = float;
    using Type = std::array<ValueType, n>;
    const size_t s = sizeof(ValueType) * n;

    LOG() << "stack size of std::array<float, 9> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti, to;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            ti[idx] = ValueType(idx);
        }

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::array<float, 9> instance is: " <<
            size << std::endl;
    }

    {
        char buf[s];
        Type t1, t2;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            t1[idx] = ValueType(idx);
        }

        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[s];
        Type t1, t2;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            t1[idx] = ValueType(idx);
        }

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

TEST(RawStlContainer, String)
{
    const size_t n = 9;
    using ValueType = char;
    using Type = std::string;
    const size_t s = sizeof(size_t) + sizeof(ValueType) * n;

    LOG() << "stack size of std::string is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti = "3.1415926", to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::string instance is: " <<
            size << std::endl;
    }

    {
        char buf[s];
        Type t1 = "3.1415926", t2;
        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[s];
        Type t1 = "3.1415926", t2;

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

TEST(RawStlSequential, Vector)
{
    const size_t n = 1000;
    using ValueType = int;
    using Type = std::vector<ValueType>;
    const size_t s = sizeof(size_t) + sizeof(ValueType) * n;

    LOG() << "stack size of std::vector<int> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti(n), to;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            ti[idx] = ValueType(idx);
        }

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::vector<int> instance is: " <<
            size << std::endl;
    }

    {
        char buf[s];
        Type t1(n), t2;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            t1[idx] = ValueType(idx);
        }

        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[s];
        Type t1(n), t2;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            t1[idx] = ValueType(idx);
        }

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

TEST(RawStlSequential, VectorBench)
{
    const size_t n = 100000000;
    using ValueType = int;
    using Type = std::vector<ValueType>;
    const size_t s = sizeof(size_t) + sizeof(ValueType) * n;

    LOG() << "stack size of std::vector<int> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        auto buf = std::unique_ptr<char[]>(new char[s]);
        Type ti(n), to;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            ti[idx] = ValueType(idx);
        }

        auto t0 = std::chrono::system_clock::now();
        auto size = serialized_size(ti);
        auto t1 = std::chrono::system_clock::now();
        auto pi = serialize(buf.get(), ti);
        auto t2 = std::chrono::system_clock::now();
        auto po = deserialize(buf.get(), to);
        auto t3 = std::chrono::system_clock::now();

        LOG() << "+" << std::chrono::duration<double, std::micro>(t1 - t0).count()
              << "us +" << std::chrono::duration<double, std::micro>(t2 - t1).count()
              << "us +" << std::chrono::duration<double, std::micro>(t3 - t2).count()
              << "us" << std::endl;

        EXPECT_EQ(size, s);
        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        LOG() << "serialized size of std::vector<int> instance is: " <<
            size << std::endl;
    }
}

TEST(RawStlSequential, VectorVector)
{
    const size_t n = 1000;
    using ValueType = float;
    using Type = std::vector<std::vector<ValueType>>;
    const size_t s = sizeof(size_t) * (1 + n) + sizeof(ValueType) * n * (n - 1) / 2;

    LOG() << "stack size of std::vector<std::vector<float>> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti, to;

        for (size_t idx_outter = 0; idx_outter < size_t(n); ++idx_outter)
        {
            std::vector<ValueType> item;
            for (size_t idx_inner = 0; idx_inner < size_t(idx_outter); ++idx_inner)
            {
                item.emplace_back(ValueType(idx_outter * 10 + idx_inner));
            }
            ti.emplace_back(item);
        }

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::vector<std::vector<float>> instance is: " <<
            size << std::endl;
    }

    {
        char buf[s];
        Type t1, t2;

        for (size_t idx_outter = 0; idx_outter < size_t(n); ++idx_outter)
        {
            std::vector<ValueType> item;
            for (size_t idx_inner = 0; idx_inner < size_t(idx_outter); ++idx_inner)
            {
                item.emplace_back(ValueType(idx_outter * 10 + idx_inner));
            }
            t1.emplace_back(item);
        }

        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[s];
        Type t1, t2;

        for (size_t idx_outter = 0; idx_outter < size_t(n); ++idx_outter)
        {
            std::vector<ValueType> item;
            for (size_t idx_inner = 0; idx_inner < size_t(idx_outter); ++idx_inner)
            {
                item.emplace_back(ValueType(idx_outter * 10 + idx_inner));
            }
            t1.emplace_back(item);
        }

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

TEST(RawStlSequential, Deque)
{
    const size_t n = 9;
    using ValueType = int;
    using Type = std::deque<ValueType>;
    const size_t s = sizeof(size_t) + sizeof(ValueType) * n;

    LOG() << "stack size of std::deque<int> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti(n), to;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            ti[idx] = ValueType(idx);
        }

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::deque<int> instance is: " <<
            size << std::endl;
    }

    {
        char buf[s];
        Type t1(n), t2;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            t1[idx] = ValueType(idx);
        }

        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[s];
        Type t1(n), t2;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            t1[idx] = ValueType(idx);
        }

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

TEST(RawStlSequential, ForwardList)
{
    const size_t n = 9;
    using ValueType = float;
    using Type = std::forward_list<ValueType>;
    const size_t s = sizeof(size_t) + sizeof(ValueType) * n;

    LOG() << "stack size of std::forward_list<float> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti, to;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            ti.emplace_front(ValueType(idx));
        }

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::forward_list<float> instance is: " <<
            size << std::endl;
    }

    {
        char buf[s];
        Type t1, t2;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            t1.emplace_front(ValueType(idx));
        }

        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[s];
        Type t1, t2;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            t1.emplace_front(ValueType(idx));
        }

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

TEST(RawStlSequential, List)
{
    const size_t n = 9;
    using ValueType = float;
    using Type = std::list<ValueType>;
    const size_t s = sizeof(size_t) + sizeof(ValueType) * n;

    LOG() << "stack size of std::list<float> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti, to;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            ti.emplace_front(ValueType(idx));
        }

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::list<float> instance is: " <<
            size << std::endl;
    }

    {
        char buf[s];
        Type t1, t2;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            t1.emplace_front(ValueType(idx));
        }

        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[s];
        Type t1, t2;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            t1.emplace_front(ValueType(idx));
        }

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

TEST(RawStlSequential, SetFastCopy)
{
    const size_t n = 7;
    using ValueType = float;
    using Type = std::set<ValueType>;
    const size_t s = sizeof(size_t) + sizeof(ValueType) * n;

    LOG() << "stack size of std::set<float> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti = { 3.f, 1.f, 4.f, 1.f, 5.f, 9.f, 2.f, 6.f, 5.f }, to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::set<float> instance is: " <<
            size << std::endl;
    }

    {
        char buf[s];
        Type t1 = { 3.f, 1.f, 4.f, 1.f, 5.f, 9.f, 2.f, 6.f, 5.f }, t2;
        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[s];
        Type t1 = { 3.f, 1.f, 4.f, 1.f, 5.f, 9.f, 2.f, 6.f, 5.f }, t2;

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

TEST(RawStlSequential, MultiSetStandard)
{
    using ValueType = std::string;
    using Type = std::multiset<ValueType>;
    const size_t s = sizeof(size_t) * 5 + 9;

    LOG() << "stack size of std::multiset<std::string> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti = { "2", "23", "233", "233" }, to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::multiset<std::string> instance is: " <<
            size << std::endl;
    }

    {
        char buf[s];
        Type t1 = { "2", "23", "233", "233" }, t2;
        Type &ti = t1, &to = t2;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));
    }

    {
        char buf[s];
        Type t1 = { "2", "23", "233", "233" }, t2;

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

TEST(RawStlAssociative, MapFastCopy)
{
    const size_t n = 2;
    using Type = std::map<float, bool>;
    const size_t s = sizeof(size_t) + sizeof(Type::value_type) * n;

    LOG() << "stack size of std::map<float, bool> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti = { { 5.f, false }, { 9.f, true } }, to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::map<float, bool> instance is: " <<
            size << std::endl;
    }
}

TEST(RawStlAssociative, MultiMapStandard)
{
    const size_t n = 2;
    using Type = std::map<int, std::string>;
    const size_t s = sizeof(size_t) * (1 + n) + sizeof(Type::key_type) * n + 14;

    LOG() << "stack size of std::map<int, std::string> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti = { { 5, "12345" }, { 9, "123456789" } }, to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::map<int, std::string> instance is: " <<
            size << std::endl;
    }
}

TEST(RawStlAssociative, UnorderedMap)
{
    const size_t n = 2;
    using Type = std::unordered_map<std::string, int>;
    const size_t s = sizeof(size_t) * (1 + n) + sizeof(Type::mapped_type) * n + 14;

    LOG() << "stack size of std::unordered_map<std::string, int> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti = { { "12345", 5 }, { "123456789", 9 } }, to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::unordered_map<std::string, int> instance is: " <<
            size << std::endl;
    }
}

TEST(RawStlAssociative, UnorderedMultiMap)
{
    const size_t n = 3;
    using Type = std::unordered_multimap<std::string, int>;
    const size_t s = sizeof(size_t) * (1 + n) + sizeof(Type::mapped_type) * n + 19;

    LOG() << "stack size of std::unordered_multimap<std::string, int> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti = { { "12345", 5 }, { "12345", 666 }, { "123456789", 9 } };
        Type to = { { "12345", 66 }, { "12345", 666 }, { "123456789", 9 } };

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() <<
            "serialized size of std::unordered_multimap<std::string, int> instance is: " <<
            size << std::endl;
    }
}

TEST(RawStlInitializerList, InitializerList)
{
    const size_t n = 9;
    using ValueType = float;
    using Type = std::initializer_list<std::string>;
    const size_t s = sizeof(size_t) + sizeof(ValueType) * n;

    LOG() << "stack size of std::initializer_list<float> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    auto result0 = is_serialization_copyable<Type>::value;
    auto result1 = is_serializable<Type>::value;

    LOG() << "std::initializer_list<std::string> is serializable: " << result1 << std::endl; ///

    EXPECT_EQ(result0, false);
    EXPECT_EQ(result1, false);

    /*
    {
        char buf[s];
        Type ti = { "0", "1", "2" }, to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.size(), to.size());
        for (size_t idx = 0; idx < ti.size(); ++idx)
        {
            EXPECT_EQ(ti.begin() + idx, to.begin() + idx);
        }

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::initializer_list<float> instance is: " <<
            size << std::endl;
    }
    */
}

TEST(RawStlAdaptor, Stack)
{
    const size_t n = 9;
    using ValueType = int;
    using Type = std::stack<ValueType>;
    const size_t s = sizeof(size_t) + sizeof(ValueType) * n;

    LOG() << "stack size of std::stack<int> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti, to;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            ti.push(ValueType(idx));
        }

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::vector<int> instance is: " <<
            size << std::endl;
    }
}

TEST(RawStlAdaptor, Queue)
{
    const size_t n = 9;
    using ValueType = float;
    using Type = std::queue<ValueType>;
    const size_t s = sizeof(size_t) + sizeof(ValueType) * n;

    LOG() << "stack size of std::queue<float> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti, to;

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            ti.push(ValueType(idx));
        }

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::queue<float> instance is: " <<
            size << std::endl;
    }
}

TEST(RawStlAdaptor, PriorityQueue)
{
    const size_t n = 9;
    using ValueType = int;
    using Type = std::priority_queue<ValueType>;
    const size_t s = sizeof(size_t) + sizeof(ValueType) * n;

    LOG() << "stack size of std::priority_queue<int> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    auto result0 = is_serialization_copyable<Type>::value;
    auto result1 = is_serializable<Type>::value;

    LOG() << "std::priority_queue<int> is serializable: " << result1 << std::endl; ///

    EXPECT_EQ(result0, false);
    EXPECT_EQ(result1, false);

    /*
    {
        char buf[s];
        Type ti, to;
        const ValueType list[n] = { 3, 1, 4, 1, 5, 9, 2, 6 };

        for (size_t idx = 0; idx < size_t(n); ++idx)
        {
            ti.push(ValueType(list[idx]));
        }

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti, to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::priority_queue<int> instance is: " <<
            size << std::endl;
    }
    */
}

TEST(RawStlValArray, ValArray)
{
    const size_t n = 9;
    using ValueType = float;
    using Type = std::valarray<ValueType>;
    const size_t s = sizeof(size_t) + sizeof(ValueType) * n;

    LOG() << "stack size of std::valarray<float> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti = { 3.f, 1.f, 4.f, 1.f, 5.f, 9.f, 2.f, 6.f, 5.f }, to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.size(), to.size());
        for (size_t idx = 0; idx < ti.size(); ++idx)
        {
            EXPECT_EQ(ti[idx], to[idx]);
        }

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::valarray<float> instance is: " <<
            size << std::endl;
    }
}

TEST(RawStlValArray, Slice)
{
    using Type = std::slice;
    const size_t s = sizeof(Type);

    LOG() << "stack size of std::slice is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti = { 1, 3, 2 }, to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.start(), to.start());
        EXPECT_EQ(ti.size(), to.size());
        EXPECT_EQ(ti.stride(), to.stride());

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::slice instance is: " <<
            size << std::endl;
    }
}

TEST(RawStlValArray, GSlice)
{
    using Type = std::gslice;
    const size_t s = sizeof(size_t) * 7;

    LOG() << "stack size of std::gslice is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti = { 1, { 5, 5 }, { 1, 2 } }, to;

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(ti.start(), to.start());
        EXPECT_EQ(ti.size()[0], to.size()[0]);
        EXPECT_EQ(ti.size()[1], to.size()[1]);
        EXPECT_EQ(ti.stride()[0], to.stride()[0]);
        EXPECT_EQ(ti.stride()[1], to.stride()[1]);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::gslice instance is: " <<
            size << std::endl;
    }
}

TEST(RawStlValArray, SliceArray)
{
    ///
}

TEST(RawStlManaged, UniquePtr)
{
    using ValueType = std::string;
    using Type = std::unique_ptr<ValueType>;
    const size_t s = sizeof(bool) + sizeof(size_t) + 6;

    LOG() << "stack size of std::unique_ptr<std::string> is: " <<
        sizeof(Type) << std::endl;
    LOG() << "buffer size is: " << s << std::endl;

    {
        char buf[s];
        Type ti, to;

        ti.reset(new std::string { "Hahaha" });

        auto pi = serialize(buf, ti);
        auto po = deserialize(buf, to);

        EXPECT_EQ(pi, po);
        EXPECT_EQ(*ti, *to);

        auto size = serialized_size(ti);

        EXPECT_EQ(size, sizeof(buf));

        LOG() << "serialized size of std::unique_ptr<std::string> instance is: " <<
            size << std::endl;
    }
}

} // namespace NAMESPACE

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
