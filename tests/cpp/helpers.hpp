#pragma once

#include <functional>

class HashableObject {
public:
    HashableObject(int a, int b) : a(a), b(b) {}
    int a, b;
    friend struct std::hash<HashableObject>;
};

namespace std {
    template <>
    struct hash<HashableObject> {
    std::size_t operator()(const HashableObject& o) const {
            std::size_t h1 = std::hash<int>{}(o.a);
            std::size_t h2 = std::hash<int>{}(o.b);
            return h1 ^ (h2 << 1);
        }
    };
}

class UnhashableObject {
public:
    UnhashableObject(int a, int b) : a(a), b(b) {}
    int a, b;
};