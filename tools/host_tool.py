from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives.asymmetric import padding, rsa
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.backends import default_backend

KEY = b"1234567890abcdef"
IV = b"fedcba0987654321"

def encrypt_firmware(input_file, output_file):
    with open(input_file, 'rb') as f:
        plaintext = f.read()

    pad_len = 16 - (len(plaintext) % 16)
    plaintext += bytes([pad_len]) * pad_len

    cipher = Cipher(algorithms.AES(KEY), modes.CBC(IV), backend=default_backend())
    encryptor = cipher.encryptor()
    ciphertext = encryptor.update(plaintext) + encryptor.finalize()

    with open(output_file, 'wb') as f:
        f.write(ciphertext)

    print(f"Encrypted length: {len(ciphertext)}")
    return ciphertext

def sign_data(data, key_path):
    with open(key_path, "rb") as key_file:
        private_key = serialization.load_pem_private_key(key_file.read(), password=None)
    signature = private_key.sign(
        data,
        padding.PKCS1v15(),
        hashes.SHA256()
    )
    return signature

def main():
    ciphertext = encrypt_firmware("firmware.bin", "mock_flash.bin")
    sig = sign_data(ciphertext, "keys/private.pem")
    with open("mock_flash.bin", "ab") as f:
        f.write(sig)
    print("✅ Firmware encrypted and signed!")

if __name__ == "__main__":
    main()

