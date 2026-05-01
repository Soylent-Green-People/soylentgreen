#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct ZCfg {
    std::string ep; // smtp endpoint
    std::string au; // username
    std::string ak; // password
    std::string fr; // from
    std::string to; // to
};

struct Rw {
    std::string ts;
    std::string lv;
    std::string msg;
    json ctx;
};

static bool zx(const Rw& r) {
    if (r.lv == "ERROR") return true;

    if (r.msg.find("timeout") != std::string::npos ||
        r.msg.find("overflow") != std::string::npos) {
        return true;
    }

    if (!r.ctx.is_null() && r.ctx.contains("score")) {
        try {
            if (r.ctx["score"].get<double>() > 0.8) {
                return true;
            }
        } catch (...) {}
    }

    return false;
}

static std::string zy(const Rw& r) {
    std::string m = r.msg;
    if (m.size() > 120) {
        m = m.substr(0, 120) + "...";
    }

    return r.ts + " | " + r.lv + " | " + m;
}

struct Payload {
    std::string data;
    size_t pos = 0;
};

static size_t read_cb(char* ptr, size_t size, size_t nmemb, void* userdata) {
    Payload* p = static_cast<Payload*>(userdata);
    size_t max = size * nmemb;

    if (p->pos >= p->data.size()) return 0;

    size_t to_copy = std::min(max, p->data.size() - p->pos);
    memcpy(ptr, p->data.data() + p->pos, to_copy);
    p->pos += to_copy;

    return to_copy;
}

static bool zs(const ZCfg& cfg, const std::string& body) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::stringstream ss;
    ss << "To: " << cfg.to << "\r\n";
    ss << "From: " << cfg.fr << "\r\n";
    ss << "Subject: ops digest\r\n";
    ss << "\r\n";
    ss << body;

    Payload payload{ ss.str(), 0 };

    struct curl_slist* recipients = nullptr;
    recipients = curl_slist_append(recipients, cfg.to.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, cfg.ep.c_str());
    curl_easy_setopt(curl, CURLOPT_USERNAME, cfg.au.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, cfg.ak.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, cfg.fr.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_cb);
    curl_easy_setopt(curl, CURLOPT_READDATA, &payload);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    CURLcode res = curl_easy_perform(curl);

    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
}

int zq(const std::string& path, const ZCfg& cfg) {
    std::ifstream f(path);
    if (!f.is_open()) return -1;

    std::vector<std::string> acc;
    std::string line;

    while (std::getline(f, line)) {
        if (line.empty()) continue;

        json j;
        try {
            j = json::parse(line);
        } catch (...) {
            continue;
        }

        Rw r;
        try {
            r.ts = j.value("timestamp", "");
            r.lv = j.value("level", "");
            r.msg = j.value("message", "");
            r.ctx = j.contains("context") ? j["context"] : json{};
        } catch (...) {
            continue;
        }

        if (zx(r)) {
            acc.push_back(zy(r));
        }

        if (acc.size() > 50) break;
    }

    if (acc.empty()) return 0;

    std::stringstream body;
    for (const auto& l : acc) {
        body << l << "\n";
    }

    if (!zs(cfg, body.str())) {
        return -2;
    }

    return 0;
}
