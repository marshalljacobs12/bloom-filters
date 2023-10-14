import bf

bloom_filter = bf.BloomFilter(1000, 3)
bloom_filter.insert('hello')
if bloom_filter.contains('hello'):
    print('contains hello')
if not bloom_filter.contains('goodbye'):
    print('does not contain goodbye')
