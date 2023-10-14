#pragma once

#include <Bitmap.hpp>
#include <memory>
#include <functional>
#include <stdexcept>

// for debugging
#include <iostream>

// Declare the BloomFilter template class
template<typename T>
class BloomFilter;

#ifdef INCLUDE_PYBIND
#include <pybind11/pybind11.h>
template <>
class BloomFilter<pybind11::object> {
public:
    BloomFilter(size_t size, int num_hashes) : m_size(size), m_num_hashes(num_hashes) {
        m_bitmap = std::make_unique<Bitmap>(m_size);
        if (m_num_hashes > 100) {
            m_num_hashes = 100;
        }
    }
    void insert(pybind11::object item) {
        ensure_hashable(item);
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % m_size;
            m_bitmap.get()->set(hash_val);
        }
    }
    bool contains(pybind11::object item) {
        ensure_hashable(item);
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % m_size;
            if (!m_bitmap.get()->test(hash_val)) {
                return false;
            }
        }
        return true;
    }

private:
    size_t m_size;
    int m_num_hashes;
    std::unique_ptr<Bitmap> m_bitmap;
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

template <typename T>
class BloomFilter {
public:
    BloomFilter(size_t size, int num_hashes) : m_size(size), m_num_hashes(num_hashes) {
        static_assert(is_hashable<T>::value,"Type T must be hashable with std::hash!");
        m_bitmap = std::make_unique<Bitmap>(m_size);
        if (m_num_hashes > 100) {
            m_num_hashes = 100;
        }
    }
    void insert(T item) {
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % m_size;
            m_bitmap.get()->set(hash_val);
        }
    }
    bool contains(T item) {
        for (int i = 0; i < m_num_hashes; i++) {
            size_t hash_val = (hash1(item) + i * hash2(item)) % m_size;
            if (!m_bitmap.get()->test(hash_val)) {
                return false;
            }
        }
        return true;
    }

private:
    size_t m_size;
    int m_num_hashes;
    std::unique_ptr<Bitmap> m_bitmap;

    size_t hash1(const T &input) const {
        return std::hash<T>{}(input) % m_size;
    }

    size_t hash2(const T &input) const {
        return (std::hash<T>{}(input) * 0xdeadbeef) % m_size;
    }
};
#endif
