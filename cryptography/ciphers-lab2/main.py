from ciphers import affine_cipher, hill_cipher, shift_cipher, latin_with_space, latin

def check_cipher(cipher, cipher_name):
    key_file_name = "input/" + cipher_name + "/key.txt"
    encrypted_file_name = "output/" + cipher_name + "/crypt.txt"
    decrypted_file_name = "output/" + cipher_name + "/decrypt.txt"
    hack_file_name = "output/" + cipher_name + "/hack.txt"

    with open("in.txt", "r") as input_file:
        lines = input_file.read().splitlines()

    with open(key_file_name, "r") as key_file:
        key = tuple(map(int, key_file.readline().split(" ")))

    encrypted_lines = cipher.encrypt_lines(lines, *key)

    with open(encrypted_file_name, "w") as encrypted_file:
        encrypted_file.write("\n".join(encrypted_lines))

    decrypted_lines = cipher.decrypt_lines(encrypted_lines, *key)

    with open(decrypted_file_name, "w") as decrypted_file:
        decrypted_file.write("\n".join(decrypted_lines))

    hack_results = cipher.hack(encrypted_lines, latin_with_space)

    with open(hack_file_name, "w") as hack_file:
        for hack_result in hack_results:
            hack_file.write("key=" + str(hack_result[0]) + ":\n")
            hack_file.write("\n".join(hack_result[1]) + "\n\n")

def hack_file(cipher, cipher_name):
    encrypted_file = "encrypted_" + cipher_name + ".txt"
    hack_file_name = "hacked/" + cipher_name + "/hack.txt"

    with open(encrypted_file, "r") as input_file:
        lines = input_file.read().upper().splitlines()

    hack_results = cipher.hack(lines, latin, 3)

    with open(hack_file_name, "w") as hack_file:
        for hack_result in hack_results:
            hack_file.write("key=" + str(hack_result[0]) + ":\n")
            hack_file.write("\n".join(hack_result[1]) + "\n\n")

if __name__ == "__main__":    
    # check_cipher(shift_cipher, "shift_cipher")
    # check_cipher(affine_cipher, "affine_cipher")
    # check_cipher(hill_cipher, "hill_cipher")

    hack_file(shift_cipher, "shift_cipher")
    hack_file(affine_cipher, "affine_cipher")
    # hack_file(hill_cipher, "hill_cipher")



