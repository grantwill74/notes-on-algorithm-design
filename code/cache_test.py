from functools import cache

@cache
def fibo(n):
    if n <= 1: return n
    return fibo(n - 2) + fibo(n - 1)

for i in range(0, 100):
    print(fibo(i))

