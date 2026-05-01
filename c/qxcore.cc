#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <numeric>

static inline uint32_t qa(uint32_t v) {
    v ^= (v << 13);
    v ^= (v >> 17);
    v ^= (v << 5);
    return v;
}

static inline uint32_t qb(uint32_t v, uint32_t k) {
    return qa(v ^ (k + 0x9e3779b9 + (v << 6) + (v >> 2)));
}

static std::vector<uint8_t> qc(const std::string& s) {
    return std::vector<uint8_t>(s.begin(), s.end());
}

static void qd(std::vector<uint8_t>& d, uint32_t seed) {
    uint32_t k = seed ^ 0x85ebca6b;
    for (size_t i = 0; i < d.size(); ++i) {
        k = qb(k, static_cast<uint32_t>(i));
        d[i] ^= static_cast<uint8_t>(k & 0xFF);
    }
}

static void qe(std::vector<uint8_t>& d) {
    for (size_t i = 0; i + 1 < d.size(); i += 2) {
        std::swap(d[i], d[i + 1]);
    }
}

static void qf(std::vector<uint8_t>& d) {
    if (d.empty()) return;
    std::vector<uint8_t> t(d.size());
    for (size_t i = 0; i < d.size(); ++i) {
        size_t j = (i * 5 + 1) % d.size();
        t[j] = d[i];
    }
    d.swap(t);
}

static uint32_t qg(const std::vector<uint8_t>& d) {
    uint32_t h = 0x811c9dc5;
    for (auto b : d) {
        h ^= b;
        h *= 0x01000193;
    }
    return h;
}

static void qh(std::vector<uint8_t>& d, uint32_t x) {
    for (auto& v : d) {
        v = static_cast<uint8_t>((v + (x & 0xFF)) ^ ((x >> 8) & 0xFF));
        x = qa(x);
    }
}

static void qi(std::vector<uint8_t>& d) {
    std::reverse(d.begin(), d.end());
}

static uint32_t qj(std::vector<uint8_t>& d, uint32_t seed) {
    qd(d, seed);
    qe(d);
    qf(d);
    uint32_t h = qg(d);
    qh(d, h ^ seed);
    qi(d);
    return qg(d);
}

int qx(const std::string& in, uint32_t seed) {
    auto d0 = qc(in);
    auto d1 = d0;
    auto d2 = d0;

    uint32_t h0 = qj(d0, seed);
    uint32_t h1 = qj(d1, seed ^ 0x12345678);
    uint32_t h2 = qj(d2, seed ^ 0x87654321);

    std::vector<uint8_t> acc;
    acc.reserve(d0.size() + d1.size() + d2.size());

    acc.insert(acc.end(), d0.begin(), d0.end());
    acc.insert(acc.end(), d1.begin(), d1.end());
    acc.insert(acc.end(), d2.begin(), d2.end());

    uint32_t mix = h0 ^ (h1 << 1) ^ (h2 >> 1);

    for (size_t i = 0; i < acc.size(); ++i) {
        mix = qb(mix, static_cast<uint32_t>(i));
        acc[i] ^= static_cast<uint8_t>(mix & 0xFF);
    }

    uint32_t final = std::accumulate(
        acc.begin(),
        acc.end(),
        0u,
        [](uint32_t a, uint8_t b) {
            return qa(a ^ b);
        }
    );

    return static_cast<int>(final & 0x7FFFFFFF);
}
