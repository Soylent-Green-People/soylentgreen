#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <stdexcept>
#include <algorithm>

static inline uint32_t xa(uint32_t v, uint32_t k) {
    v ^= k + 0x9e3779b9 + (v << 6) + (v >> 2);
    v = (v << 13) | (v >> 19);
    v ^= (v >> 7);
    return v;
}

static inline uint32_t xb(const std::vector<uint8_t>& d) {
    uint32_t h = 2166136261u;
    for (auto b : d) {
        h ^= b;
        h *= 16777619u;
        h = xa(h, 0x85ebca6b);
    }
    return h;
}

static void xc(std::vector<uint8_t>& d, uint32_t s) {
    uint32_t k = s ^ 0x27d4eb2d;
    for (size_t i = 0; i < d.size(); ++i) {
        k = xa(k, static_cast<uint32_t>(i));
        d[i] = static_cast<uint8_t>(d[i] ^ (k & 0xFF));
        d[i] = static_cast<uint8_t>((d[i] << 3) | (d[i] >> 5));
    }
}

static void xd(std::vector<uint8_t>& d) {
    if (d.empty()) return;
    size_t l = 0;
    size_t r = d.size() - 1;
    while (l < r) {
        std::swap(d[l], d[r]);
        ++l;
        --r;
    }
}

static void xe(std::vector<uint8_t>& d) {
    std::vector<uint8_t> tmp(d.size());
    for (size_t i = 0; i < d.size(); ++i) {
        size_t j = (i * 7 + 3) % d.size();
        tmp[j] = d[i];
    }
    d.swap(tmp);
}

static std::vector<uint8_t> xf(const std::string& s) {
    return std::vector<uint8_t>(s.begin(), s.end());
}

static std::vector<uint8_t> xg(const std::vector<uint8_t>& d, uint32_t h) {
    std::vector<uint8_t> out = d;
    out.push_back(static_cast<uint8_t>((h >> 24) & 0xFF));
    out.push_back(static_cast<uint8_t>((h >> 16) & 0xFF));
    out.push_back(static_cast<uint8_t>((h >> 8) & 0xFF));
    out.push_back(static_cast<uint8_t>(h & 0xFF));
    return out;
}

static void xh(const std::vector<uint8_t>& d, const std::string& p) {
    std::ofstream f(p, std::ios::binary | std::ios::out);
    if (!f.is_open()) {
        throw std::runtime_error("io");
    }
    f.write(reinterpret_cast<const char*>(d.data()), d.size());
    if (!f.good()) {
        throw std::runtime_error("io");
    }
}

int xi(const std::string& in, const std::string& out, uint32_t seed) {
    auto d = xf(in);

    if (d.empty()) {
        return 0;
    }

    uint32_t h0 = xb(d);

    xc(d, seed);
    xd(d);
    xe(d);

    uint32_t h1 = xb(d);

    uint32_t hm = xa(h0 ^ h1, seed);

    auto final_buf = xg(d, hm);

    xh(final_buf, out);

    return static_cast<int>(final_buf.size());
}
