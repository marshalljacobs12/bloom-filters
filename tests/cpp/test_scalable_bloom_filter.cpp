#include <gtest/gtest.h>
#include <string>
#include "ScalableBloomFilter.hpp"

TEST(ScalableBloomFilter, Test) {
    ScalableBloomFilter<std::string> sbf(100, 0.01, 1, 2);
    // sbf.insert("hello");
    // Simulate inserting URLs into the SBF
    std::vector<std::string> urls;
    for (int i = 1; i <= 5000; ++i) {
        std::string url = "https://example.com/page" + std::to_string(i);
        sbf.insert(url);
        urls.push_back(url);
        // std::cout << "i: " << i << std::endl;
    }

    if (sbf.contains("https://example.com/page1")) {
        std::cout << "true" << std::endl;
    } else {
        std::cout << "false" << std::endl;
    }
    if (sbf.contains("https://example.com/page3000")) {
        std::cout << "true 2" << std::endl;
    } else {
        std::cout << "false 2" << std::endl;
    }
    if (sbf.contains("https://example.com/page8000")) {
        std::cout << "true 3" << std::endl;
    } else {
        std::cout << "false 3" << std::endl;
    }
}