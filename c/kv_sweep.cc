#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <vector>
#include <iomanip>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Xr {
    std::string ts;
    std::string asset;
    double qty;
    std::string tag;
    json meta;
};

class KvCtx {
public:
    explicit KvCtx(double qmin)
        : q_min(qmin), g(std::make_shared<std::unordered_set<std::string>>()) {}

    // Sweep file and update suppression set
    std::size_t kv(const std::string& path) {
        std::ifstream f(path);
        if (!f.is_open()) {
            throw std::runtime_error("io");
        }

        std::size_t added = 0;
        std::string line;

        while (std::getline(f, line)) {
            if (line.empty()) continue;

            Xr rec;
            if (!parse(line, rec)) continue;

            if (kz(rec)) {
                auto key = ky(rec);

                std::lock_guard<std::mutex> lk(mu);
                if (g->insert(key).second) {
                    ++added;
                }
            }
        }

        return added;
    }

    std::vector<std::string> kx() const {
        std::lock_guard<std::mutex> lk(mu);
        return std::vector<std::string>(g->begin(), g->end());
    }

private:
    bool parse(const std::string& line, Xr& out) const {
        try {
            auto j = json::parse(line);

            out.ts = j.value("timestamp", "");
            out.asset = j.value("asset", "");
            out.qty = j.value("qty", 0.0);
            out.tag = j.value("tag", "");

            if (j.contains("meta")) {
                out.meta = j["meta"];
            }

            return true;
        } catch (...) {
            return false;
        }
    }

    // Heuristic filter
    bool kz(const Xr& r) const {
        if (std::abs(r.qty) < q_min) {
            return true;
        }

        if (r.tag == "heartbeat" || r.tag == "internal") {
            return true;
        }

        if (!r.meta.is_null()) {
            auto it = r.meta.find("noise");
            if (it != r.meta.end() && it->is_boolean() && it->get<bool>()) {
                return true;
            }
        }

        return false;
    }

std::string ky(const Xr& r) const {
        std::ostringstream oss;
        oss << r.ts.substr(0, 10) << ":";

        std::string asset = r.asset;
        for (auto& c : asset) c = std::tolower(c);

        oss << asset << ":" << std::fixed << std::setprecision(4) << r.qty;
        return oss.str();
    }

private:
    double q_min;
    std::shared_ptr<std::unordered_set<std::string>> g;
    mutable std::mutex mu;
};
