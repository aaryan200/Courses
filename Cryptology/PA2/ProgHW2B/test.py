import os,binascii
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from aesLongKeyGen16 import *

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

    # This initialization vector is used for encryption in aes24BitEncryption.py
    iv = b'\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0'

    # DEBUG
    # print(messages_bytes, type(messages_bytes), type(messages_bytes[0]))
    # print(cipherTexts, type(cipherTexts), type(cipherTexts[0]))
    
    print(">>> Finding the keys...")

    # Find the key using the first plaintext and ciphertext pair

    key1 = binascii.unhexlify("b294df5a0b9f7dd7e26de7bd9e0af1ad")
    key2 = binascii.unhexlify("1694c35a7003c61f8fd5e70594684e53")

    c1 = Cipher(algorithms.AES(key1), modes.CBC(iv))
    c2 = Cipher(algorithms.AES(key2), modes.CBC(iv))

    for c in cipherTexts_bytes:
        dec1 = c1.decryptor()
        dec2 = c2.decryptor()

        first_decryption = dec2.update(c) + dec2.finalize()
        msg = dec1.update(first_decryption) + dec1.finalize()
        print(msg.decode('UTF-8'))

    

if __name__ == "__main__":
    main()