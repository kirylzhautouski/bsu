from primes import *

# https://psv4.userapi.com/c834704/u190970339/docs/d18/bf68711735b8/Bryus_Shnayer_Prikladnaya_kriptografia_Protokoly_algoritmy_iskhodnye_texty_na_yazyke_Si.pdf?extra=hpMu2W7K1z4iZ50zBD5N8dGAIc9_H2TOCZJEwhiXVhdkiY9qqyqbi6k2Nitnxe22g39o12p2zmEFUueeImXzm2Wgx0YDLwEirqpQHnWtbUwfhgsaM7n0Yf2jhbBbEc6u0bDZ6rbUQ_LRy0hE6wdmwkM#page=362&zoom=100,0,925
# https://vk.com/doc8457127_487346179 - Брюс Шнайер. Прикладная криптография
# https://en.wikipedia.org/wiki/Jacobi_symbol
# https://en.wikipedia.org/wiki/Solovay%E2%80%93Strassen_primality_test
# https://en.wikipedia.org/wiki/Fermat_primality_test
# https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test
# https://habr.com/en/post/205318/

def check_tests(n):    
    for i in range(2, n):
        if naive_is_prime(i):
            fermat_result = fermat_test(i)
            miller_rabin_result = miller_rabin(i)
            solovay_strassen_test_result = solovay_strassen_test(i)

            if fermat_result == False or miller_rabin_result == False or \
                solovay_strassen_test_result == False:
                print(i)

                print("Fermat: " + str(fermat_result))
                print("Miller-Rabin: " + str(miller_rabin_result))
                print("Solovay-Strassen: " + str(solovay_strassen_test_result))

                print()

def run_tests(n):
    print("Fermat: " + str(fermat_test(n)))
    print("Miller-Rabin: " + str(miller_rabin(n)))
    print("Solovay-Strassen: " + str(solovay_strassen_test(n)))

if __name__ == "__main__":
    number = generate_prime(256)
    
    print()
    print()
    print(number)
    run_tests(number)
    
