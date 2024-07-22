"""
Aaryan
CO21BTECH11001
CS6160: Cryptology
ProgHW2B: findKey_hw2b.py
"""

import os,binascii
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from aesLongKeyGen16 import *

def find_keys(iv, message_bytes, cipherText_bytes):
    """
    Parameters:
        iv (bytes): The initialization vector used for encryption
        message_bytes (bytes): The plaintext
        cipherText_bytes (bytes): The final ciphertext
    Returns:
        tuple: The keys used to encrypt the plaintext to the ciphertext1 and the ciphertext1 to the final ciphertext.

    If the keys are not found, return None.
    """
    plain_to_cipher1 = [] # List of the ciphertexts that can be generated from encryption of the plaintext using all possible 2-byte keys
    
    # Iterate k1 through all possible 2-byte keys
    for k1 in range(0, 1 << 16):
        key = k1.to_bytes(2, "big")
        # Expand the key to 16 bytes
        expKey = expandKey(key)

        cipher = Cipher(algorithms.AES(expKey), modes.CBC(iv))
        encryptor = cipher.encryptor()
        cipherText = encryptor.update(message_bytes) + encryptor.finalize()
        # Append the ciphertext and the corresponding 16-byte key to the list
        plain_to_cipher1.append([cipherText, expKey])
    
    # Sort the list according to the ciphertext
    plain_to_cipher1.sort(key=lambda x: x[0])

    final_cipher_to_cipher1 = [] # List of the ciphertexts that can be generated from decryption of the final ciphertext using all possible 2-byte keys

    # Iterate k2 through all possible 2-byte keys
    for k2 in range(0, 1 << 16):
        key = k2.to_bytes(2, "big")
        # Expand the key to 16 bytes
        expKey = expandKey(key)

        cipher = Cipher(algorithms.AES(expKey), modes.CBC(iv))
        decryptor = cipher.decryptor()
        decrypted_cipher = decryptor.update(cipherText_bytes) + decryptor.finalize()
        # Append the ciphertext and the corresponding 16-byte key to the list
        final_cipher_to_cipher1.append([decrypted_cipher, expKey])
    
    # Sort the list according to the ciphertext
    final_cipher_to_cipher1.sort(key=lambda x: x[0])

    # Indexes for iterating through both lists
    i = 0
    j = 0

    # Iterate through both lists to find the keys
    while i < len(plain_to_cipher1) and j < len(final_cipher_to_cipher1):

        if (plain_to_cipher1[i][0] == final_cipher_to_cipher1[j][0]):
            # If the ciphertexts match, then the keys are found
            return (plain_to_cipher1[i][1], final_cipher_to_cipher1[j][1])
        
        elif (plain_to_cipher1[i][0] < final_cipher_to_cipher1[j][0]):
            # If the ciphertext from plain_to_cipher1 is smaller than the ciphertext from final_cipher_to_cipher1, then increment i
            i += 1

        else:
            # If the ciphertext from plain_to_cipher1 is larger than the ciphertext from final_cipher_to_cipher1, then increment j
            j += 1

    # If the keys are not found, return None
    return None


def main():
    with open("2aesPlaintexts.txt","r") as reader:
        messages=reader.read().split('\n')

    with open("2aesCiphertexts.txt","r") as reader:
        cipherTexts=reader.read().split('\n')
    
    print(">>> Done reading messages and ciphertexts from file")

    # DEBUG
    # print(messages)
    # print(cipherTexts)
    
    # Convert messages and cipherTexts to bytes
    messages_bytes = [message.encode('UTF-8') for message in messages]
    cipherTexts_bytes = [binascii.unhexlify(cipherText) for cipherText in cipherTexts]

    # This initialization vector is used for encryption in 2aes16BitEncryption.py
    iv = b'\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0'

    # DEBUG
    # print(messages_bytes, type(messages_bytes), type(messages_bytes[0]))
    # print(cipherTexts, type(cipherTexts), type(cipherTexts[0]))
    
    print(">>> Finding the keys...")

    # Find the key using the first plaintext and ciphertext pair
    keys = find_keys(iv, messages_bytes[0], cipherTexts_bytes[0])

    if (keys != None):
        key1, key2 = keys
        print("Keys found:\n")
        print(f"Plain to cipher key: {key1.hex()}")
        print(f"Cipher to cipher key: {key2.hex()}")

        # Find the secret message
        secret_cipherText = cipherTexts_bytes[-1]

        decryptor1 = Cipher(algorithms.AES(key1), modes.CBC(iv)).decryptor()
        decryptor2 = Cipher(algorithms.AES(key2), modes.CBC(iv)).decryptor()

        # Decrypt from the final ciphertext to the intermediate ciphertext
        first_decryption = decryptor2.update(secret_cipherText) + decryptor2.finalize()
        # Decrypt from the intermediate ciphertext to the secret message
        secret_message = decryptor1.update(first_decryption) + decryptor1.finalize()

        print("\nSecret message:", secret_message.decode('UTF-8'))

        # Write the keys and secret message to file
        with open("output.txt", "w") as writer:
            writer.write(f"Plain to cipher key: {key1.hex()}")
            writer.write(f"\nCipher to cipher key: {key2.hex()}")
            writer.write(f"\nSecret message: {secret_message.decode('UTF-8')}")

        print("\nWritten keys and secret message to output.txt")

    else: 
        print("Keys not found")

if __name__ == "__main__":
    main()