#include <gtest/gtest.h>
#include <helpers.hpp>
#include <CountingBloomFilter.hpp>
#include <string>

TEST(CountingBloomFilter, ConstructorCppType) {
    CountingBloomFilter<int, 1000> cbf(3);
    cbf.insert(42);
    ASSERT_TRUE(cbf.contains(42));
}

TEST(CountingBloomFilter, ConstructorCppClass) {
    CountingBloomFilter<std::string, 100> cbf(3);
    cbf.insert("42");
    ASSERT_TRUE(cbf.contains("42"));
}

TEST(CountingBloomFilter, ConstructorHashableClass) {
    CountingBloomFilter<HashableObject, 100> cbf(3);
    HashableObject o(1, 2);
    cbf.insert(o);
    ASSERT_TRUE(cbf.contains(o));
}

TEST(CountingBloomFilter, Contains) {
    CountingBloomFilter<std::string, 100> cbf(3);
    cbf.insert("42");
    ASSERT_TRUE(cbf.contains("42"));
    ASSERT_FALSE(cbf.contains("43"));
}

TEST(CountingBloomFilter, Insert) {
    CountingBloomFilter<std::string, 100> cbf( 3);
    cbf.insert("42");
    cbf.insert("43");
    cbf.insert("44");
    cbf.insert("45");
    ASSERT_TRUE(cbf.contains("42")); 
    ASSERT_TRUE(cbf.contains("43"));
    ASSERT_TRUE(cbf.contains("44"));
    ASSERT_TRUE(cbf.contains("45"));
}

TEST(CountingBloomFilter, InsertRepeatedValue) {
    CountingBloomFilter<std::string, 100> cbf(3);
    cbf.insert("42");
    cbf.insert("42");
    ASSERT_TRUE(cbf.contains("42"));
    ASSERT_EQ(cbf.count("42"), 2);
}

TEST(CountingBloomFilter, Remove) {
    CountingBloomFilter<std::string, 100> cbf(3);
    cbf.insert("42");
    cbf.remove("42");
    ASSERT_FALSE(cbf.contains("42"));

    cbf.insert("42");
    cbf.insert("42");
    cbf.remove("42");
    ASSERT_TRUE(cbf.contains("42"));
}

TEST(CountingBloomFilter, Count) {
    CountingBloomFilter<std::string, 100> cbf(3);
    cbf.insert("42");
    cbf.insert("42");
    cbf.insert("43");
    ASSERT_EQ(cbf.count("42"), 2);
    ASSERT_EQ(cbf.count("43"), 1);
    ASSERT_EQ(cbf.count("44"), 0);
}

TEST(CountingBloomFilter, ContainsFalsePositive) {
    CountingBloomFilter<int, 100> cbf(3);
    for (int i = 0; i < 10000; i++) {
        cbf.insert(i);
    }
    ASSERT_TRUE(cbf.contains(10001));
}
