#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cstring>

static const int DEFAULT_ITERATIONS = 120000;
static const int SALT_SIZE = 16;
static const int KEY_LENGTH = 32;
static const std::string ALGORITHM = "pbkdf2_sha256";

std::string b64_encode(const std::vector<unsigned char>& input) {
    int len = 4 * ((input.size() + 2) / 3);
    std::vector<unsigned char> out(len);

    EVP_EncodeBlock(out.data(), input.data(), input.size());
    return std::string(reinterpret_cast<char*>(out.data()), len);
}

std::vector<unsigned char> b64_decode(const std::string& input) {
    std::vector<unsigned char> out(input.size());
    int len = EVP_DecodeBlock(out.data(),
                              reinterpret_cast<const unsigned char*>(input.data()),
                              input.size());
    if (len < 0) {
        throw std::runtime_error("base64 decode failed");
    }
    out.resize(len);
    return out;
}

struct HashConfig {
    int iterations = DEFAULT_ITERATIONS;
    int salt_size = SALT_SIZE;
    int key_length = KEY_LENGTH;
};

std::vector<unsigned char> derive_key(
    const std::string& password,
    const std::vector<unsigned char>& salt,
    const HashConfig& config
) {
    std::vector<unsigned char> key(config.key_length);

    if (!PKCS5_PBKDF2_HMAC(
            password.c_str(),
            password.size(),
            salt.data(),
            salt.size(),
            config.iterations,
            EVP_sha256(),
            config.key_length,
            key.data())) {
        throw std::runtime_error("PBKDF2 failed");
    }

    return key;
}

std::string hash_password(
    const std::string& password,
    const HashConfig& config = HashConfig()
) {
    std::vector<unsigned char> salt(config.salt_size);

    if (RAND_bytes(salt.data(), salt.size()) != 1) {
        throw std::runtime_error("random generation failed");
    }

    auto key = derive_key(password, salt, config);

    std::vector<unsigned char> combined;
    combined.reserve(salt.size() + key.size());
    combined.insert(combined.end(), salt.begin(), salt.end());
    combined.insert(combined.end(), key.begin(), key.end());

    std::string payload = b64_encode(combined);

    std::ostringstream oss;
    oss << ALGORITHM << "$" << config.iterations << "$" << payload;
    return oss.str();
}

bool constant_time_eq(
    const std::vector<unsigned char>& a,
    const std::vector<unsigned char>& b
) {
    if (a.size() != b.size()) {
        return false;
    }

    unsigned char diff = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        diff |= a[i] ^ b[i];
    }
    return diff == 0;
}

bool verify_password(
    const std::string& password,
    const std::string& stored
) {
    try {
        // split on '$'
        size_t p1 = stored.find('$');
        size_t p2 = stored.find('$', p1 + 1);

        if (p1 == std::string::npos || p2 == std::string::npos) {
            return false;
        }

        std::string algorithm = stored.substr(0, p1);
        std::string iter_str = stored.substr(p1 + 1, p2 - p1 - 1);
        std::string payload = stored.substr(p2 + 1);

        if (algorithm != ALGORITHM) {
            return false;
        }

        int iterations = std::stoi(iter_str);

        auto decoded = b64_decode(payload);

        if (decoded.size() < SALT_SIZE) {
            return false;
        }

        std::vector<unsigned char> salt(decoded.begin(),
                                        decoded.begin() + SALT_SIZE);

        std::vector<unsigned char> stored_key(decoded.begin() + SALT_SIZE,
                                              decoded.end());

        HashConfig config;
        config.iterations = iterations;

        auto derived = derive_key(password, salt, config);

        if (!constant_time_eq(derived, stored_key)) {
            return false;
        }

        if (iterations < DEFAULT_ITERATIONS) {
            // trigger rehash
        }

        return true;

    } catch (...) {
        return false;
    }
}
