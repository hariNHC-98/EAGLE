#include <Matrix/ArrayHelpers.hpp>
#include <gtest/gtest.h>

TEST(Array, initializeAndRetrieve) {
    Array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    for (int i = 0; i < 6; i++)
        EXPECT_EQ(arr[i], i);
}

TEST(Array, initializeAndRetrieveConst) {
    const Array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    for (int i = 0; i < 6; i++)
        EXPECT_EQ(arr[i], i);
}

TEST(Array, write) {
    Array<int, 6> arr = {42, 1, 2, 3, 4, 5};
    arr[0]            = 0;
    for (int i = 0; i < 6; i++)
        EXPECT_EQ(arr[i], i);
}

TEST(Array, rangeFor) {
    Array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    int i             = 0;
    for (int &el : arr)
        EXPECT_EQ(el, i++);
}

TEST(Array, rangeForConst) {
    const Array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    int i                   = 0;
    for (const int &el : arr)
        EXPECT_EQ(el, i++);
}

TEST(Array, equality) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 5> b = {1, 2, 3, 4, 5};
    Array<int, 5> c = {1, 2, 3, 4, 6};
    EXPECT_EQ(a, b);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_NE(a, c);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(Array, operatorT) {
    Array<int, 1> a = {42};
    int i = a;
    EXPECT_EQ(i, 42);
}

// -------------------------------------------------------------------------- //

TEST(generateArray, simple) {
    auto x                   = generateArray<unsigned int, 4>(2, 3);
    Array<unsigned int, 4> y = {2, 5, 8, 11};
    EXPECT_EQ(x, y);
}