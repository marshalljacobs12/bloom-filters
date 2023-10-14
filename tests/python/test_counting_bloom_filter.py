import pytest
import bf
from .helpers import Hashable
import threading
import time

def test_counting_bloom_filter_constructor_int():
    counting_bloom_filter = bf.CountingBloomFilter(100, 3)
    counting_bloom_filter.insert(3)
    assert counting_bloom_filter.contains(3) == True

def test_counting_bloom_filter_constructor_str():
    counting_bloom_filter = bf.CountingBloomFilter(100, 3)
    counting_bloom_filter.insert("hello")
    assert counting_bloom_filter.contains("hello") == True

def test_counting_bloom_filter_constructor_tuple():
    counting_bloom_filter = bf.CountingBloomFilter(100, 3)
    counting_bloom_filter.insert((1, 2))
    assert counting_bloom_filter.contains((1, 2)) == True

def test_counting_bloom_filter_hashable():
    counting_bloom_filter = bf.CountingBloomFilter(100, 3)
    counting_bloom_filter.insert(Hashable(1, 2))
    assert counting_bloom_filter.contains(Hashable(1, 2)) == True

def test_counting_bloom_filter_nonhashable():
    counting_bloom_filter = bf.CountingBloomFilter(100, 3)
    with pytest.raises(RuntimeError):
        counting_bloom_filter.insert([1, 2])

def test_counting_bloom_filter_contains():
    counting_bloom_filter = bf.CountingBloomFilter(100, 3)
    counting_bloom_filter.insert(3)
    assert counting_bloom_filter.contains(3) == True

def test_counting_bloom_filter_contains_false():
    counting_bloom_filter = bf.CountingBloomFilter(100, 3)
    assert counting_bloom_filter.contains(3) == False

def test_counting_bloom_filter_insert():
    counting_bloom_filter = bf.CountingBloomFilter(100, 3)
    counting_bloom_filter.insert(3)
    assert counting_bloom_filter.contains(3) == True

def test_counting_bloom_filter_insert_repeated_values():
    counting_bloom_filter = bf.CountingBloomFilter(100, 3)
    counting_bloom_filter.insert(3)
    counting_bloom_filter.insert(3)
    assert counting_bloom_filter.contains(3) == True
    assert counting_bloom_filter.count(3) == 2

def test_counting_bloom_filter_remove():
    counting_bloom_filter = bf.CountingBloomFilter(100, 3)
    counting_bloom_filter.insert(3)
    counting_bloom_filter.remove(3)
    assert counting_bloom_filter.contains(3) == False
    counting_bloom_filter.insert(3)
    counting_bloom_filter.insert(3)
    counting_bloom_filter.remove(3)
    assert counting_bloom_filter.contains(3) == True
    assert counting_bloom_filter.count(3) == 1

def test_counting_bloom_filter_count():
    counting_bloom_filter = bf.CountingBloomFilter(100, 3)
    counting_bloom_filter.insert(3)
    counting_bloom_filter.insert(3)
    counting_bloom_filter.insert(4)
    assert counting_bloom_filter.count(3) == 2
    assert counting_bloom_filter.count(4) == 1
    assert counting_bloom_filter.count(5) == 0

def test_counting_bloom_filter_contains_false_positive():
    counting_bloom_filter = bf.CountingBloomFilter(100, 3)
    for i in range(10000):
        counting_bloom_filter.insert(i)
    assert counting_bloom_filter.contains(10001) == True

def test_counting_bloom_filter_thread_safety_insert():
    counting_bloom_filter = bf.CountingBloomFilter(10000, 10)
    threads = []

    def worker():
        for _ in range(1000):
            counting_bloom_filter.insert(42)
    
    for _ in range(4):
        thread = threading.Thread(target=worker)
        thread.start()
        threads.append(thread)

    for thread in threads:
        thread.join()
    
    assert counting_bloom_filter.count(42) == 4000

def test_counting_bloom_filter_thread_safety_remove():
    counting_bloom_filter = bf.CountingBloomFilter(10000, 10)
    threads = []

    for _ in range(4000):
        counting_bloom_filter.insert(42)

    def worker():
        for _ in range(1000):
            counting_bloom_filter.remove(42)
    
    for _ in range(4):
        thread = threading.Thread(target=worker)
        thread.start()
        threads.append(thread)

    for thread in threads:
        thread.join()
    
    assert counting_bloom_filter.count(42) == 0