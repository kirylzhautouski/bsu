# import enchant

# d = enchant.Dict("en_US")

def gcd_extended(a, b):
    if b == 0:
        return a, 1, 0

    d, ap, bp = gcd_extended(b, a % b)

    return (d, bp, ap - a // b * bp)

def find_inverse(a, m):
    d, x, _ = gcd_extended(a, m)

    if d == 1:
        return x % m

    return None

def gcd(a, b):   
    if a < b:
        return gcd(b, a)

    if b == 0:
        return a

    return gcd(b, a % b)

# def is_english(line):
#     if line == None:
#         return False
 
#     count_english = 0
#     l = len(line)

#     for i in range(l // 4):
#             for j in range(i + 3, i + 8):
#                 try:
#                     if d.check(line[i:j]):
#                         count_english += 1
#                         break
#                 except ValueError:
#                     continue    
#
#     return True if count_english >= l / 16 else False