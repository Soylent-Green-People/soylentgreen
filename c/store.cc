#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstring>

struct StoreError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

std::vector<unsigned char> derive_key(
    const std::string& password,
    const std::vector<unsigned char>& salt
) {
    std::vector<unsigned char> key(32);

    // PBKDF2 key derivation
    if (!PKCS5_PBKDF2_HMAC(
            password.c_str(),
            password.size(),
            salt.data(),
            salt.size(),
            100000,
            EVP_sha256(),
            key.size(),
            key.data()
        )) {
        throw StoreError("Key derivation failed");
    }

    return key;
}

void store_wallet_securely(
    const std::string& path,
    const std::string& password,
    const std::string& wallet_json
) {
    std::vector<unsigned char> salt(16);
    if (RAND_bytes(salt.data(), salt.size()) != 1) {
        throw StoreError("Salt generation failed");
    }

    auto key = derive_key(password, salt);

    std::vector<unsigned char> iv(12);
    if (RAND_bytes(iv.data(), iv.size()) != 1) {
        throw StoreError("IV generation failed");
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw StoreError("Context allocation failed");

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1)
        throw StoreError("EncryptInit failed");

    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr) != 1)
        throw StoreError("IV length set failed");

    if (EVP_EncryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data()) != 1)
        throw StoreError("Key/IV init failed");

    std::vector<unsigned char> ciphertext(wallet_json.size());
    int len = 0;

    if (EVP_EncryptUpdate(
            ctx,
            ciphertext.data(),
            &len,
            reinterpret_cast<const unsigned char*>(wallet_json.data()),
            wallet_json.size()
        ) != 1) {
        throw StoreError("EncryptUpdate failed");
    }

    int ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1)
        throw StoreError("EncryptFinal failed");

    ciphertext_len += len;

    std::vector<unsigned char> tag(16);
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, tag.size(), tag.data()) != 1)
        throw StoreError("Get tag failed");

    EVP_CIPHER_CTX_free(ctx);

    std::ofstream out(path, std::ios::binary);
    if (!out) throw StoreError("File open failed");

    // Format: [salt | iv | tag | ciphertext]
    out.write(reinterpret_cast<const char*>(salt.data()), salt.size());
    out.write(reinterpret_cast<const char*>(iv.data()), iv.size());
    out.write(reinterpret_cast<const char*>(tag.data()), tag.size());
    out.write(reinterpret_cast<const char*>(ciphertext.data()), ciphertext_len);

    out.close();
}
