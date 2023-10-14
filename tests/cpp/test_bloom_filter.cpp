#include <gtest/gtest.h>
#include <helpers.hpp>
#include <BloomFilter.hpp>
#include <string>

TEST(BloomFilter, ConstructorCppType) {
    BloomFilter<int> bf(100, 3);
    bf.insert(42);
    ASSERT_TRUE(bf.contains(43));
}

TEST(BloomFilter, ConstructorCppClass) {
    BloomFilter<std::string> bf(100, 3);
    bf.insert("42");
    ASSERT_TRUE(bf.contains("42"));
}

TEST(BloomFilter, ConstructorHashableClass) {
    BloomFilter<HashableObject> bf(100, 3);
    HashableObject o(1, 2);
    bf.insert(o);
    ASSERT_TRUE(bf.contains(o));
}

TEST(BloomFilter, Contains) {
    BloomFilter<std::string> bf(100, 3);
    bf.insert("42");
    ASSERT_TRUE(bf.contains("42"));
    ASSERT_FALSE(bf.contains("43"));
}

TEST(BloomFilter, Insert) {
    BloomFilter<std::string> bf(100, 3);
    bf.insert("42");
    bf.insert("43");
    bf.insert("44");
    bf.insert("45");
    ASSERT_TRUE(bf.contains("42")); 
    ASSERT_TRUE(bf.contains("43"));
    ASSERT_TRUE(bf.contains("44"));
    ASSERT_TRUE(bf.contains("45"));
}

TEST(BloomFilter, ContainsFalsePositive) {
    BloomFilter<int> bf(100, 3);
    for (int i = 0; i < 10000; i++) {
        bf.insert(i);
    }
    ASSERT_TRUE(bf.contains(10001));
}