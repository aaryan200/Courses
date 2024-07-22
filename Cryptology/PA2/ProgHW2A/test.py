import os,binascii
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from aesLongKeyGen24 import *


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
    key = binascii.unhexlify("8e94635ae87bde371e30e71d3b6b516e")

    for cb in cipherTexts_bytes:
        decryptor = Cipher(algorithms.AES(key), modes.CBC(iv)).decryptor()
        msg = decryptor.update(cb) + decryptor.finalize()
        print(msg.decode('UTF-8'))

if __name__ == "__main__":
    main()