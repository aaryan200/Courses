"""
Aaryan
CO21BTECH11001
CS6160: Cryptology
ProgHW2A: findKey_hw2a.py
"""

import os,binascii
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from aesLongKeyGen24 import *

def encrypt_message(key, iv, message_bytes):
    """
    Encrypt the message_bytes using the key and iv and return the ciphertext.
    AES-128-CBC mode is used for encryption
    """
    cipher = Cipher(algorithms.AES(key), modes.CBC(iv))

    encryptor = cipher.encryptor()
    ciphertext = encryptor.update(message_bytes) + encryptor.finalize()

    return ciphertext


def is_valid_key(key, iv, message_bytes, actual_cipherText_bytes):
    """
    Returns True if the key is valid, False otherwise
    """
    computed_cipherText = encrypt_message(key, iv, message_bytes)

    # DEBUG
    # print(f"{computed_cipherText.hex()}\n{actual_cipherText.hex()}\n")

    if (computed_cipherText == actual_cipherText_bytes):
        return True
    
    return False


def find_key(iv, message_bytes, actual_cipherText_bytes):
    """
    Given the message_bytes and actual_cipherText_bytes, find the key by brute force attack.
    Returns the key if found, None otherwise.
    """
    # Iterate from 0 to 2^20
    for i in range(0, 1 << 20):
        # Since last 4 bits are ignored in expansion subroutine, we shift i by 4 bits to the left
        key = i << 4
        # Convert key to 3 bytes
        key = key.to_bytes(3, "big")
        # Expand key to 128 bits
        exp_key = expandKey(key)

        # Check if the key is valid
        if (is_valid_key(exp_key, iv, message_bytes, actual_cipherText_bytes)):
            return exp_key
        
    return None

def main():
    with open("aesPlaintexts.txt","r") as reader:
        messages=reader.read().split('\n')

    with open("aesCiphertexts.txt","r") as reader:
        cipherTexts=reader.read().split('\n')
    
    print(">>> Done reading messages and ciphertexts from file")

    # DEBUG
    # print(messages)
    # print(cipherTexts)
    
    # Convert messages and cipherTexts to bytes
    messages_bytes = [message.encode('UTF-8') for message in messages]
    cipherTexts_bytes = [binascii.unhexlify(cipherText) for cipherText in cipherTexts]

    # This initialization vector is used for encryption in aes24BitEncryption.py
    iv = b'\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0'

    # DEBUG
    # print(messages_bytes, type(messages_bytes), type(messages_bytes[0]))
    # print(cipherTexts, type(cipherTexts), type(cipherTexts[0]))
    
    print(">>> Finding the key...")

    # Find the key using the first plaintext and ciphertext pair
    key = find_key(iv, messages_bytes[0], cipherTexts_bytes[0])

    if (key != None):
        print("\nKey found:", key.hex())

        # Find the secret message
        secret_cipherText = cipherTexts_bytes[-1]

        decryptor = Cipher(algorithms.AES(key), modes.CBC(iv)).decryptor()
        secret_message = decryptor.update(secret_cipherText) + decryptor.finalize()

        print("\nSecret message:", secret_message.decode('UTF-8'))

        # Write the key and secret message to file
        with open("output.txt", "w") as writer:
            writer.write(f"Key: {key.hex()}")
            writer.write(f"\nSecret message: {secret_message.decode('UTF-8')}")

        print("\nWritten key and secret message to output.txt")

    else: 
        print("Key not found")

if __name__ == "__main__":
    main()