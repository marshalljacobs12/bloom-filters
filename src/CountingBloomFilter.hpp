#pragma once

#include <array>
#include <memory>
#include <functional>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <mutex>

// for debugging
#include <iostream>

template<typename T, std::size_t N=0>
class CountingBloomFilter;

#ifdef INCLUDE_PYBIND
#include <pybind11/pybind11.h>
template <>
class CountingBloomFilter<pybind11::object> {
public:
    CountingBloomFilter(size_t size, int num_hashes) {
        m_num_hashes = num_hashes;
        if (m_num_hashes > 100) {
            m_num_hashes = 100;
        }
        m_size = size;
        m_filter.resize(m_size); // initially fill with zeros
        std::fill(m_filter.begin(), m_filter.end(), 0);
    }   
    void insert(pybind11::object item) {
        ensure_hashable(item);
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % m_size;
            ++m_filter[hash_val];
        }
    }
    bool contains(pybind11::object item) {
        ensure_hashable(item);
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % m_size;
             if (m_filter[hash_val] == 0) {
                return false;
            }
        }
        return true;
    }

    void remove(pybind11::object item) {
        ensure_hashable(item);
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        if (!contains(item)) {
            return;
        }
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % m_size;
            --m_filter[hash_val];
        }
    }

    int count(pybind11::object item) {
        ensure_hashable(item);
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        if (!contains(item)) {
            return 0;
        }

        int min_count = std::numeric_limits<int>::max();
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % m_size;
            min_count = std::min(min_count, m_filter[hash_val]);
        }

        return min_count;
    }

private:
    int m_num_hashes;
    size_t m_size;
    std::vector<int> m_filter;
    std::recursive_mutex m_mutex;
    
    void ensure_hashable(const pybind11::object input) {
        try {
            pybind11::hash(input);
        } catch(...) {
            throw std::runtime_error("Object is not hashable!");
        }
    }

    size_t hash1(const pybind11::object &input) const {
        pybind11::object hash_obj = pybind11::module::import("builtins").attr("hash")(input);
        long hash_value = hash_obj.cast<long>();
        return std::hash<long>()(hash_value) % m_size;
    }

    size_t hash2(const pybind11::object &input) const {
        pybind11::object hash_obj = pybind11::module::import("builtins").attr("hash")(input);
        long hash_value = hash_obj.cast<long>();
        return (std::hash<long>()(hash_value) * 0xdeadbeef) % m_size;
    }
};
#endif

#ifndef INCLUDE_PYBIND

template <typename T, typename = void>
struct is_hashable : std::false_type {};

template <typename T>
struct is_hashable<T, decltype(std::hash<T>{}(std::declval<T>()), void())> : std::true_type {};

template <typename T, std::size_t N>
class CountingBloomFilter {
public:
    CountingBloomFilter(int num_hashes) {
        static_assert(N != 0, "N must be > 0");
        m_num_hashes = num_hashes;
        if (m_num_hashes > 100) {
            m_num_hashes = 100;
        }
        m_filter.fill(0); // initially fill with zeros
    }

    void insert(const T& item) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % N;
            ++m_filter[hash_val];
        }
    }
    bool contains(const T& item) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % N;
            if (m_filter[hash_val] == 0) {
                return false;
            }
        }
        return true;
    }

    void remove(const T& item) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        if (!contains(item)) {
            return;
        }
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % N;
            --m_filter[hash_val];
        }
    }

    int count(const T& item) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        if (!contains(item)) {
            return 0;
        }

        int min_count = std::numeric_limits<int>::max();
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % N;
            min_count = std::min(min_count, m_filter[hash_val]);
        }
        return min_count;
    }

private:
    int m_num_hashes;
    std::array<int, N> m_filter;
    std::recursive_mutex m_mutex;

    size_t hash1(const T &input) const {
        return std::hash<T>{}(input) % N;
    }

    size_t hash2(const T &input) const {
        return (std::hash<T>{}(input) * 0xdeadbeef) % N;
    }
};
#endif
