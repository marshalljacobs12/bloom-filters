#pragma once

#include <array>
#include <memory>
#include <functional>
#include <stdexcept>

// for debugging
#include <iostream>

template<typename T, std::size_t N>
class CountingBloomFilter;

#ifdef INCLUDE_PYBIND
#include <pybind11/pybind11.h>
template <std::size_t N>
class CountingBloomFilter<pybind11::object, N> {
public:
    CountingBloomFilter(int num_hashes) {
        m_num_hashes = num_hashes;
        if (m_num_hashes > 100) {
            m_num_hashes = 100;
        }
        m_filter.fill(0); // initially fill with zeros
    }
    void insert(pybind11::object item) {
        ensure_hashable(item);
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % N;
            ++m_filter[hash_val];
        }
    }
    bool contains(pybind11::object item) {
        ensure_hashable(item);
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % N;
             if (m_filter[hash_val] == 0) {
                return false;
            }
        }
        return true;
    }

    void remove(pybind11::object item) {
        ensure_hashable(item);
        if (!contains(item)) {
            return;
        }
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % N;
            --m_filter[hash_val];
        }
    }

    int count(pybind11::object item) {
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
        return std::hash<long>()(hash_value) % N;
    }

    size_t hash2(const pybind11::object &input) const {
        pybind11::object hash_obj = pybind11::module::import("builtins").attr("hash")(input);
        long hash_value = hash_obj.cast<long>();
        return (std::hash<long>()(hash_value) * 0xdeadbeef) % N;
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
        m_num_hashes = num_hashes;
        if (m_num_hashes > 100) {
            m_num_hashes = 100;
        }
        m_filter.fill(0); // initially fill with zeros
    }
    void insert(const T& item) {
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % N;
            ++m_filter[hash_val];
        }
    }
    bool contains(const T& item) const {
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % N;
            if (m_filter[hash_val] == 0) {
                return false;
            }
        }
        return true;
    }

    void remove(const T& item) {
        if (!contains(item)) {
            return;
        }
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % N;
            --m_filter[hash_val];
        }
    }

    int count(const T& item) const {
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

    size_t hash1(const T &input) const {
        return std::hash<T>{}(input) % N;
    }

    size_t hash2(const T &input) const {
        return (std::hash<T>{}(input) * 0xdeadbeef) % N;
    }
};
#endif
