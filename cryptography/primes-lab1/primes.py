from random import randint, random
from math import gcd, sqrt

def generate_prime(number_of_bits=256):
    threshold = 1 << number_of_bits

    q = 5
    n = 2

    p = q * n + 1
    while p < threshold:
        q = p
        n = 2
        p = q * n + 1

        while p >= (2 * q + 1) ** 2 or pow(2, q * n, p) != 1 or pow(2, n, p) == 1:
            n += 2
            p = q * n + 1

    return p

def generate_prime_with_tests(number_of_bits=256):
    n = 1
    n <<= number_of_bits
    n ^= 1

    for i in range(1, number_of_bits - 1):
        if random() > 0.5:
            n ^= 1 <<  i

    print(bin(n))
    print()
    
    while miller_rabin(n, 5) != True:
        n += 2

    return n

def naive_is_prime(num):
    if num % 2 == 0 or num % 3 == 0:
        return False

    i = 5
    while i * i <= num:
        if num % i == 0 or num % (i + 2) == 0:
            return False

        i = i + 6

    return True

def fermat_test(num, number_of_iterations=50):
    # a ** (n-1) === 1 (modn)
    if num == 2:
        return True

    if num % 2 == 0:
        return False

    for i in range(number_of_iterations):
        a = randint(2, num - 1)

        if pow(a, num - 1, num) != 1:
            return False

    return True

def miller_rabin_one_iteration(num, d):
    a = randint(2, num - 1); 
  
    x = pow(a, d, num); 
  
    if x == 1 or x == num - 1: 
        return True 
  
    while d != num - 1: 
        x = (x * x) % num
        d *= 2
  
        if x == 1: 
            return False
        if x == num - 1: 
            return True
  
    return False

def miller_rabin(num, number_of_iterations=50):
    if num == 2:
        return True

    if num % 2 == 0:
        return False
    
    d = num - 1
    while d % 2 == 0: 
        d //= 2
  
    for i in range(number_of_iterations): 
        if miller_rabin_one_iteration(num, d) == False: 
            return False
  
    return True

def jacobi(a, b):
    if a >= b:
        a %= b
    
    if a == 0:
        return 0

    if a == 1:
        return 1

    if a < 0:
        if (b - 1) // 2 % 2 == 0:
            return jacobi(-a, b)
        else:
            return -jacobi(-a, b)

    if a % 2 == 0:
        if ((b * b - 1 ) // 8) % 2 == 0:
            return jacobi(a // 2, b)
        else:
            return -jacobi(a // 2, b)

    g = gcd(a, b)

    if g == a:
        return 0
    elif g != 1:
        return jacobi(g, b) * jacobi(a / g, b)
    elif ((a - 1) * (b - 1) // 4) % 2 == 0:
        return jacobi(b, a)
    else:
        return -jacobi(b, a)

def solovay_strassen_test(num, number_of_iterations=50):
    if num == 2:
        return True

    if num % 2 == 0:
        return False

    for i in range(number_of_iterations):
        a = randint(2, num - 1)

        if gcd(a, num) != 1:
            return False

        j = pow(a, (num - 1) // 2, num) 
        if j != jacobi(a, num) % num:
            return False

    return True