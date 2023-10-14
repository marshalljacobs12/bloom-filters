import pytest
import bf
from .helpers import Hashable

def test_bloom_filter_constructor_int():
    bloom_filter = bf.BloomFilter(100, 3)
    bloom_filter.insert(3)
    assert bloom_filter.contains(3) == True

def test_bloom_filter_constructor_str():
    bloom_filter = bf.BloomFilter(100, 3)
    bloom_filter.insert("hello")
    assert bloom_filter.contains("hello") == True

def test_bloom_filter_constructor_tuple():
    bloom_filter = bf.BloomFilter(100, 3)
    bloom_filter.insert((1, 2))
    assert bloom_filter.contains((1, 2)) == True

def test_bloom_filter_hashable():
    bloom_filter = bf.BloomFilter(100, 3)
    bloom_filter.insert(Hashable(1, 2))
    assert bloom_filter.contains(Hashable(1, 2)) == True

def test_bloom_filter_nonhashable():
    bloom_filter = bf.BloomFilter(100, 3)
    with pytest.raises(RuntimeError):
        bloom_filter.insert([1, 2])

def test_bloom_filter_contains():
    bloom_filter = bf.BloomFilter(100, 3)
    bloom_filter.insert(3)
    assert bloom_filter.contains(3) == True

def test_bloom_filter_contains_false():
    bloom_filter = bf.BloomFilter(100, 3)
    assert bloom_filter.contains(3) == False

def test_bloom_filter_insert():
    bloom_filter = bf.BloomFilter(100, 3)
    bloom_filter.insert(3)
    assert bloom_filter.contains(4) == True

def test_bloom_filter_contains_false_positive():
    bloom_filter = bf.BloomFilter(100, 3)
    for i in range(10000):
        bloom_filter.insert(i)
    assert bloom_filter.contains(10001) == True
