from ciphers import latin, latin_with_space
from .utilitites import *

def encrypt_lines(lines, a, b, c, d, alphabet=latin_with_space):
    return [encrypt(line, a, b, c, d, alphabet) for line in lines]

def decrypt_lines(lines, a, b, c, d, alphabet=latin_with_space):
    decrypt_result = [None] * len(lines)
    
    for i, line in enumerate(lines):
        decrypt_result[i] = decrypt(line, a, b, c, d, alphabet)
        if decrypt_result[i] == None:
            return None
    
    return decrypt_result

def encrypt(line, a, b, c, d, alphabet):
    alphabet_size = len(alphabet.latin_alphabet)

    if len(line) % 2 == 1:
        line += alphabet.latin_alphabet[alphabet_size - 1]

    encrypted_line_builder = []

    for i in range(0, len(line) - 1, 2):
        encrypted_line_builder.append(alphabet.latin_alphabet[(a * alphabet.latin_pos[line[i]] + \
            c * alphabet.latin_pos[line[i + 1]]) \
            % alphabet_size])
        encrypted_line_builder.append(alphabet.latin_alphabet[(b * alphabet.latin_pos[line[i]] + \
            d * alphabet.latin_pos[line[i + 1]]) \
            % alphabet_size])

    return "".join(encrypted_line_builder)

def decrypt(line, a, b, c, d, alphabet):
    alphabet_size = len(alphabet.latin_alphabet)

    det_inv = find_inverse(a * d - b * c, alphabet_size)

    return encrypt(line, (d * det_inv) % alphabet_size, (-b  * det_inv) % alphabet_size, \
        (-c * det_inv) % alphabet_size, (a * det_inv) % alphabet_size, alphabet) \
            if det_inv != None else None

def check_word(h1, h2, h3, h4, hack_results_builder, lines, alphabet):
    alphabet_size = len(alphabet.latin_alphabet)

    det_inv = find_inverse(h1 * h4 - h2 * h3, alphabet_size)

    if det_inv == None:
        return
    
    h1 = (h1 * det_inv) % alphabet_size
    h2 = (-h2 * det_inv) % alphabet_size
    h3 = (-h3 * det_inv) % alphabet_size
    h4 = (h4 * det_inv) % alphabet_size

    h1, h4 = h4, h1

    for line in lines:
        for i in range(0, len(line) - 3, 2):
            x1, x2, x3, x4 = alphabet.latin_pos[line[i]], alphabet.latin_pos[line[i + 1]], \
                alphabet.latin_pos[line[i + 2]], alphabet.latin_pos[line[i + 3]]

            a = (h1 * x1 + h2 * x3) % alphabet_size  
            b = (h1 * x2 + h2 * x4) % alphabet_size
            c = (h3 * x1 + h4 * x3) % alphabet_size
            d = (h3 * x2 + h4 * x4) % alphabet_size

            hacked_lines = decrypt_lines(lines, a, b, c, d, alphabet)
            
            if hacked_lines != None:
                hack_results_builder.append(((a, b, c, d), hacked_lines))                  

def hack(lines, alphabet):
    # does not work for any text
    hint = "MORNI"
    hack_results_builder = []

    h1, h2, h3, h4 = alphabet.latin_pos[hint[0]], alphabet.latin_pos[hint[1]], \
        alphabet.latin_pos[hint[2]], alphabet.latin_pos[hint[3]]
    check_word(h1, h2, h3, h4, hack_results_builder, lines, alphabet)

    h1, h2, h3, h4 = alphabet.latin_pos[hint[1]], alphabet.latin_pos[hint[2]], \
        alphabet.latin_pos[hint[3]], alphabet.latin_pos[hint[4]]
    check_word(h1, h2, h3, h4, hack_results_builder, lines, alphabet)

    return hack_results_builder