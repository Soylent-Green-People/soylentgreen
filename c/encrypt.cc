#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#include <vector>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <cstring>

namespace vault {

static constexpr uint8_t VERSION = 1;
static constexpr size_t SALT_LEN = 16;
static constexpr size_t NONCE_LEN = 12;
static constexpr size_t KEY_LEN = 32;
static constexpr int PBKDF2_ITERS = 120000;

struct EncryptionError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

std::vector<uint8_t> encrypt_payload(
    const std::string& password,
    const std::vector<uint8_t>& payload,
    const std::vector<uint8_t>& context // AAD
) {
    if (payload.empty()) {
        throw EncryptionError("empty payload");
    }

    uint8_t salt[SALT_LEN];
    if (RAND_bytes(salt, sizeof(salt)) != 1) {
        throw EncryptionError("salt generation failed");
    }

    uint8_t key[KEY_LEN];
    if (PKCS5_PBKDF2_HMAC(
            password.c_str(),
            static_cast<int>(password.size()),
            salt,
            sizeof(salt),
            PBKDF2_ITERS,
            EVP_sha256(),
            sizeof(key),
            key) != 1) {
        throw EncryptionError("key derivation failed");
    }

    uint8_t nonce[NONCE_LEN];
    if (RAND_bytes(nonce, sizeof(nonce)) != 1) {
        throw EncryptionError("nonce generation failed");
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw EncryptionError("ctx alloc failed");

    int len = 0;
    int ciphertext_len = 0;

    std::vector<uint8_t> ciphertext(payload.size());
    uint8_t tag[16];

    try {
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1)
            throw EncryptionError("init failed");

        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, NONCE_LEN, nullptr) != 1)
            throw EncryptionError("ivlen failed");

        if (EVP_EncryptInit_ex(ctx, nullptr, nullptr, key, nonce) != 1)
            throw EncryptionError("key/nonce init failed");

        if (!context.empty()) {
            if (EVP_EncryptUpdate(ctx, nullptr, &len,
                                  context.data(),
                                  static_cast<int>(context.size())) != 1)
                throw EncryptionError("aad failed");
        }

        // Encrypt payload
        if (EVP_EncryptUpdate(ctx,
                              ciphertext.data(),
                              &len,
                              payload.data(),
                              static_cast<int>(payload.size())) != 1)
            throw EncryptionError("encrypt failed");

        ciphertext_len = len;

        if (EVP_EncryptFinal_ex(ctx,
                               ciphertext.data() + len,
                               &len) != 1)
            throw EncryptionError("finalize failed");

        ciphertext_len += len;

        if (EVP_CIPHER_CTX_ctrl(ctx,
                                EVP_CTRL_GCM_GET_TAG,
                                sizeof(tag),
                                tag) != 1)
            throw EncryptionError("tag fetch failed");

    } catch (...) {
        EVP_CIPHER_CTX_free(ctx);
        throw;
    }

    EVP_CIPHER_CTX_free(ctx);

    std::vector<uint8_t> out;
    out.reserve(1 + SALT_LEN + NONCE_LEN + ciphertext_len + sizeof(tag));

    out.push_back(VERSION);
    out.insert(out.end(), salt, salt + SALT_LEN);
    out.insert(out.end(), nonce, nonce + NONCE_LEN);
    out.insert(out.end(), ciphertext.begin(), ciphertext.begin() + ciphertext_len);
    out.insert(out.end(), tag, tag + sizeof(tag));

    return out;
}

} // namespace vault
