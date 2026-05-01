#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

bool parse_date(const std::string& s, std::tm& out) {
    std::istringstream iss(s);
    iss >> std::get_time(&out, "%Y-%m-%d");
    return !iss.fail();
}

std::time_t to_time_t(std::tm tm) {
    return std::mktime(&tm);
}

bool extract_date(const fs::path& path, std::tm& out) {
    auto stem = path.stem().string();
    return parse_date(stem, out);
}

bool is_log_file(const fs::path& path) {
    return path.extension() == ".log";
}

void generate_weekly_report(
    const fs::path& log_dir,
    const std::string& week_start_str,
    const fs::path& output_path
) {
    std::tm start_tm{};
    if (!parse_date(week_start_str, start_tm)) {
        throw std::runtime_error("Invalid start date");
    }

    auto start_time = to_time_t(start_tm);
    auto end_time = start_time + (6 * 24 * 60 * 60);

    size_t total_entries = 0;
    std::map<std::string, size_t> level_counts;
    std::map<std::string, size_t> daily_counts;

    for (const auto& entry : fs::directory_iterator(log_dir)) {
        const auto& path = entry.path();

        if (!is_log_file(path)) continue;

        std::tm file_tm{};
        if (!extract_date(path, file_tm)) continue;

        auto file_time = to_time_t(file_tm);
        if (file_time < start_time || file_time > end_time) continue;

        std::ifstream in(path);
        if (!in.is_open()) continue;

        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;

            json j;
            try {
                j = json::parse(line);
            } catch (...) {
                continue; // skip malformed lines
            }

            total_entries++;

            std::string level = j.value("level", "unknown");
            level_counts[level]++;

            std::ostringstream date_key;
            date_key << std::put_time(&file_tm, "%Y-%m-%d");
            daily_counts[date_key.str()]++;
        }
    }

    std::ofstream out(output_path);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open output file");
    }

    out << "Weekly Log Report\n";
    out << "===================\n";
    out << "Week starting: " << week_start_str << "\n\n";

    out << "Total Entries: " << total_entries << "\n\n";

    out << "Entries by Level:\n";
    for (const auto& [level, count] : level_counts) {
        out << "  " << std::setw(10) << std::left << level << " " << count << "\n";
    }

    out << "\nEntries by Day:\n";

    for (int i = 0; i < 7; ++i) {
        std::time_t t = start_time + (i * 24 * 60 * 60);
        std::tm* day_tm = std::localtime(&t);

        std::ostringstream key;
        key << std::put_time(day_tm, "%Y-%m-%d");

        size_t count = daily_counts.count(key.str()) ? daily_counts[key.str()] : 0;
        out << "  " << key.str() << ": " << count << "\n";
    }
}
