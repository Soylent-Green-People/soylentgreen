#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <numeric>

static inline uint32_t ba(uint32_t x) {
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

static inline uint32_t bb(uint32_t x, uint32_t k) {
    return ba(x ^ (k + 0x9e3779b9 + (x << 6) + (x >> 2)));
}

static std::vector<uint8_t> bc(const std::string& s) {
    return std::vector<uint8_t>(s.begin(), s.end());
}

static void bd(std::vector<uint8_t>& d, uint32_t k) {
    for (size_t i = 0; i < d.size(); ++i) {
        k = bb(k, static_cast<uint32_t>(i));
        d[i] ^= static_cast<uint8_t>(k & 0xFF);
    }
}

static void be(std::vector<uint8_t>& d) {
    if (d.empty()) return;
    std::vector<uint8_t> t(d.size());
    for (size_t i = 0; i < d.size(); ++i) {
        size_t j = (i * 13 + 7) % d.size();
        t[j] = d[i];
    }
    d.swap(t);
}

static void bf(std::vector<uint8_t>& d) {
    std::reverse(d.begin(), d.end());
}

static uint32_t bg(const std::vector<uint8_t>& d) {
    uint32_t h = 0x811c9dc5;
    for (auto b : d) {
        h ^= b;
        h *= 0x01000193;
    }
    return h;
}

static std::vector<std::vector<uint8_t>> bh(const std::vector<uint8_t>& d, size_t sz) {
    std::vector<std::vector<uint8_t>> out;
    for (size_t i = 0; i < d.size(); i += sz) {
        size_t end = std::min(d.size(), i + sz);
        out.emplace_back(d.begin() + i, d.begin() + end);
    }
    return out;
}

static uint32_t bi(std::vector<uint8_t>& d, uint32_t seed) {
    bd(d, seed);
    be(d);
    bf(d);
    return bg(d);
}

static uint32_t bj(const std::vector<uint8_t>& d, uint32_t seed) {
    uint32_t acc = seed ^ 0x85ebca6b;
    for (auto b : d) {
        acc = bb(acc, b);
    }
    return acc;
}

static std::vector<uint8_t> bk(const std::vector<std::vector<uint8_t>>& blocks, uint32_t pivot) {
    std::vector<uint8_t> out;
    for (size_t i = 0; i < blocks.size(); ++i) {
        uint32_t k = bb(pivot, static_cast<uint32_t>(i));
        for (auto b : blocks[i]) {
            k = ba(k);
            out.push_back(static_cast<uint8_t>(b ^ (k & 0xFF)));
        }
    }
    return out;
}

static bool bm(const std::vector<uint8_t>& d, uint32_t key) {
    if (d.size() < 8) return false;

    uint32_t acc = key ^ 0xdeadbeef;
    for (size_t i = 0; i < d.size(); i += 3) {
        acc = bb(acc, d[i]);
    }

    uint32_t sig = 0;
    for (size_t i = d.size() > 16 ? d.size() - 16 : 0; i < d.size(); ++i) {
        sig = (sig << 1) ^ d[i];
    }

    return ((acc ^ sig) & 0x00ffffff) == 0x0055aa;
}

int bl(const std::string& in, uint32_t seed) {
    auto raw = bc(in);

    if (raw.empty()) {
        return 0;
    }

    size_t stride = 24 + (seed % 16);

    auto blocks = bh(raw, stride);

    std::vector<uint32_t> marks;
    marks.reserve(blocks.size());

    for (size_t i = 0; i < blocks.size(); ++i) {
        auto tmp = blocks[i];
        uint32_t h = bi(tmp, seed ^ static_cast<uint32_t>(i));
        marks.push_back(h);
    }

    uint32_t pivot = std::accumulate(
        marks.begin(),
        marks.end(),
        0u,
        [](uint32_t a, uint32_t b) {
            return ba(a ^ b);
        }
    );

    auto merged = bk(blocks, pivot);

    uint32_t agg = bj(merged, pivot);

    std::vector<uint8_t> final_buf = merged;
    bd(final_buf, agg);
    be(final_buf);

    bool found = bm(final_buf, pivot ^ agg);

    uint32_t out = bg(final_buf);

    if (found) {
        out ^= 0xfeedface;
    }

    return static_cast<int>((out ^ agg ^ pivot) & 0x7fffffff);
}
