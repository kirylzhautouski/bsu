from ciphers import latin, latin_with_space
from itertools import combinations
from .utilitites import *

def encrypt_lines(lines, a, b, alphabet=latin_with_space):
    return [encrypt(line, a, b, alphabet) for line in lines]

def decrypt_lines(lines, a, b, alphabet=latin_with_space):
    return [decrypt(line, a, b, alphabet) for line in lines]

def encrypt(line, a, b, alphabet):
    encrypted_line_builder = []

    alphabet_size = len(alphabet.latin_alphabet)

    for c in line:
        encrypted_line_builder.append(alphabet.latin_alphabet[(a * alphabet.latin_pos[c] + b) \
            % alphabet_size])

    return "".join(encrypted_line_builder)

def decrypt(line, a, b, alphabet):
    decrypted_line_builder = []

    alphabet_size = len(alphabet.latin_alphabet)

    a_inv = find_inverse(a, alphabet_size)

    for c in line:
        decrypted_line_builder.append(alphabet.latin_alphabet[(a_inv * (alphabet.latin_pos[c] - b)) \
            % alphabet_size])

    return "".join(decrypted_line_builder)
  
def hack(lines, alphabet, number_of_iterations=None):
    alphabet_size = len(alphabet.latin_alphabet)
    number_of_iterations = alphabet_size if number_of_iterations == None else \
        number_of_iterations

    hack_results_builder = []

    letter_freq = { c: 0 for c in alphabet.latin_alphabet }
    for line in lines:
        for c in line:
            letter_freq[c] += 1

    most_freq_letter = max(letter_freq, key=letter_freq.get)
    del letter_freq[most_freq_letter]
    second_most_freq_letter = max(letter_freq, key=letter_freq.get)

    coprime_integers = [i for i in range(1, alphabet_size) if gcd(alphabet_size, i) == 1]

    y1 = alphabet.latin_pos[most_freq_letter]
    y2 = alphabet.latin_pos[second_most_freq_letter]
    for most_freq_latin_letter, second_most_freq_latin_letter in \
        combinations(alphabet.latin_freq[:number_of_iterations], 2):
        x1 = alphabet.latin_pos[most_freq_latin_letter]
        x2 = alphabet.latin_pos[second_most_freq_latin_letter]

        inv = find_inverse(x1 - x2, alphabet_size)
        if inv != None:
            a = ((y1 - y2) * inv) % alphabet_size
            b = (y1 - a * x1) % alphabet_size

            hacked_lines = decrypt_lines(lines, a, b, alphabet)    
            hack_results_builder.append(((most_freq_latin_letter, second_most_freq_latin_letter, \
            a, b), hacked_lines))
                  
    return hack_results_builder
