from ciphers import latin, latin_with_space
from .utilitites import *

def shift_line(line, b, alphabet):
    output_line = []
    alphabet_size = len(alphabet.latin_alphabet)

    for c in line:
        output_line.append(alphabet.latin_alphabet[(alphabet.latin_pos[c] + b) \
            % alphabet_size])

    return "".join(output_line)

def encrypt_lines(lines, b, alphabet=latin_with_space):
    return [encrypt(line, b, alphabet) for line in lines]

def decrypt_lines(lines, b, alphabet=latin_with_space):
    return [decrypt(line, b, alphabet) for line in lines]

def encrypt(line, b, alphabet):
    return shift_line(line, b, alphabet)

def decrypt(line, b, alphabet):
    return shift_line(line, -b, alphabet)

def hack(lines, alphabet, number_of_iterations=None):
    number_of_iterations = len(alphabet.latin_alphabet) if number_of_iterations == None else \
        number_of_iterations

    hack_results_builder = []

    letter_freq = { c: 0 for c in alphabet.latin_alphabet }
    for line in lines:
        for c in line:
            letter_freq[c] += 1

    most_freq_letter = max(letter_freq, key=letter_freq.get)

    for most_freq_latin_letter in alphabet.latin_freq[:number_of_iterations]:
        key = (alphabet.latin_pos[most_freq_letter] - alphabet.latin_pos[most_freq_latin_letter]) \
            % len(alphabet.latin_alphabet)

        hacked_lines = decrypt_lines(lines, key, alphabet)  

        hack_results_builder.append((key, hacked_lines))
        
    return hack_results_builder
