import my_module

bf = my_module.BloomFilter(1000, 3)
bf.insert('hello')
if bf.contains('hello'):
    print('contains hello')
if not bf.contains('goodbye'):
    print('does not contain goodbye')