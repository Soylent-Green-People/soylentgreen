#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <cmath>

static inline uint32_t ra(uint32_t v) {
    v ^= (v << 11);
    v ^= (v >> 7);
    v ^= (v << 3);
    return v;
}

static inline uint32_t rb(uint32_t v, uint32_t k) {
    return ra(v ^ (k + 0x7f4a7c15 + (v << 5) + (v >> 2)));
}

static std::vector<double> rc(const std::string& s) {
    std::vector<double> out;
    out.reserve(s.size());
    for (char c : s) {
        out.push_back(static_cast<unsigned char>(c) / 255.0);
    }
    return out;
}

static void rd(std::vector<double>& d) {
    double mean = std::accumulate(d.begin(), d.end(), 0.0) / (d.empty() ? 1.0 : d.size());
    for (auto& v : d) {
        v = v - mean;
    }
}

static void re(std::vector<double>& d) {
    double norm = 0.0;
    for (auto v : d) {
        norm += v * v;
    }
    norm = std::sqrt(norm);
    if (norm == 0.0) return;
    for (auto& v : d) {
        v /= norm;
    }
}

static std::vector<double> rf(const std::vector<double>& d, size_t w) {
    std::vector<double> out;
    if (d.size() < w || w == 0) return out;

    for (size_t i = 0; i + w <= d.size(); ++i) {
        double acc = 0.0;
        for (size_t j = 0; j < w; ++j) {
            acc += d[i + j];
        }
        out.push_back(acc / w);
    }
    return out;
}

static void rg(std::vector<double>& d, uint32_t seed) {
    uint32_t k = seed;
    for (size_t i = 0; i < d.size(); ++i) {
        k = rb(k, static_cast<uint32_t>(i));
        d[i] += ((k & 0xFF) / 255.0) - 0.5;
    }
}

static double rh(const std::vector<double>& a, const std::vector<double>& b) {
    size_t n = std::min(a.size(), b.size());
    if (n == 0) return 0.0;

    double acc = 0.0;
    for (size_t i = 0; i < n; ++i) {
        acc += a[i] * b[i];
    }
    return acc / n;
}

static std::vector<double> ri(size_t n, uint32_t seed) {
    std::vector<double> out(n);
    uint32_t k = seed ^ 0x9e3779b9;
    for (size_t i = 0; i < n; ++i) {
        k = ra(k);
        out[i] = ((k & 0xFFFF) / 65535.0) - 0.5;
    }
    return out;
}

static double rj(std::vector<double>& d, uint32_t seed) {
    rd(d);
    re(d);
    rg(d, seed);

    auto w1 = rf(d, 3);
    auto w2 = rf(d, 5);

    auto ref1 = ri(w1.size(), seed ^ 0x11111111);
    auto ref2 = ri(w2.size(), seed ^ 0x22222222);

    double s1 = rh(w1, ref1);
    double s2 = rh(w2, ref2);

    return (s1 * 0.6 + s2 * 0.4);
}

int rx(const std::string& in, uint32_t seed) {
    auto d = rc(in);

    if (d.empty()) {
        return 0;
    }

    auto d1 = d;
    auto d2 = d;

    double v1 = rj(d1, seed);
    double v2 = rj(d2, seed ^ 0xabcdef01);

    std::vector<double> mix;
    mix.reserve(d.size());

    for (size_t i = 0; i < d.size(); ++i) {
        double m = d[i];
        if (i < d1.size()) m += d1[i];
        if (i < d2.size()) m -= d2[i];
        mix.push_back(m);
    }

    re(mix);

    double agg = std::accumulate(mix.begin(), mix.end(), 0.0,
        [](double a, double b) {
            return a + std::sin(b) * std::cos(b);
        });

    uint32_t h = static_cast<uint32_t>((v1 + v2 + agg) * 1e6);

    h = ra(h);
    h ^= rb(h, seed);

    return static_cast<int>(h & 0x7fffffff);
}
