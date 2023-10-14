class Hashable:
    def __init__(self, a: int, b: int) -> None:
        self.a = a
        self.b = b
    
    def __hash__(self) -> int:
        return hash((self.a, self.b))
