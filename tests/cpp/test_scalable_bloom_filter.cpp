#include <gtest/gtest.h>
#include <string>
#include "ScalableBloomFilter.hpp"

// TEST(ScalableBloomFilter, Constructor) {
//     // ScalableBloomFilter<int> sbf(5, 1000, 0.01);
//     // sbf.insert(42);
//     // Initial SBF setup
//     int initial_size = 1000;
//     int num_partitions = 3;
//     double initial_false_positive_rate = 0.05;
//     ScalableBloomFilter<std::string> sbf(initial_size, num_partitions, initial_false_positive_rate);

//     // Simulate inserting URLs into the SBF
//     std::vector<std::string> urls;
//     for (int i = 1; i <= initial_size; ++i) {
//         std::string url = "https://example.com/page" + std::to_string(i);
//         sbf.insert(url);
//         urls.push_back(url);
//         std::cout << "i: " << i << std::endl;
//     }
//     std::cout << "Inserted " << initial_size << " URLs" << std::endl;
//     // Check false-positive rate before resizing
//     // int false_positives = 0;
//     // for (const auto& url : urls) {
//     //     if (sbf.contains("https://nonexistent.com/" + url)) {
//     //         false_positives++;
//     //     }
//     // }

//     // double false_positive_rate_before_resize = static_cast<double>(false_positives) / initial_size;
//     // std::cout << "False Positive Rate Before Resize: " << false_positive_rate_before_resize << std::endl;

//     // // Simulate increased crawling activity and insertion of more URLs
//     // int additional_urls = 500;
//     // for (int i = 1; i <= additional_urls; ++i) {
//     //     std::string url = "https://example.com/page" + std::to_string(initial_size + i);
//     //     sbf.insert(url);
//     //     urls.push_back(url);
//     // }

//     // // Check false-positive rate after resizing
//     // false_positives = 0;
//     // for (const auto& url : urls) {
//     //     if (sbf.contains("https://nonexistent.com/" + url)) {
//     //         false_positives++;
//     //     }
//     // }

//     // double false_positive_rate_after_resize = static_cast<double>(false_positives) / (initial_size + additional_urls);
//     // std::cout << "False Positive Rate After Resize: " << false_positive_rate_after_resize << std::endl;

// }

TEST(ScalableBloomFilter, Test) {
    ScalableBloomFilter sbf(100, 0.01, 1);
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