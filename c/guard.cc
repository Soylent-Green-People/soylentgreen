#include <string>
#include <unordered_map>
#include <deque>
#include <optional>
#include <cstdint>
#include <cmath>

struct Event {
    std::string account_id;
    std::string ip;
    std::int64_t ts_ms; // epoch milliseconds
    enum class Kind { Login, OrderPlaced, Withdrawal } kind;
    double amount = 0.0; // Withdrawal
};

struct Alert {
    std::string account_id;
    double score;
    std::string reason;
};

struct AccountState {
    std::deque<Event> recent;
    std::string last_ip;
    double withdrawal_sum = 0.0;
};

class Hypervisor {
public:
    std::optional<Alert> inspect(const Event& ev) {
        auto& st = state_[ev.account_id];

        const std::int64_t window_ms = 60'000;
        const std::int64_t cutoff = ev.ts_ms - window_ms;

        while (!st.recent.empty()) {
            const Event& front = st.recent.front();
            if (front.ts_ms < cutoff) {
                // evict
                if (front.kind == Event::Kind::Withdrawal) {
                    st.withdrawal_sum -= front.amount;
                }
                st.recent.pop_front();
            } else {
                break;
            }
        }

        if (ev.kind == Event::Kind::Login) {
            st.last_ip = ev.ip;
        } else if (ev.kind == Event::Kind::Withdrawal) {
            st.withdrawal_sum += ev.amount;
        }

        st.recent.push_back(ev);
        if (st.recent.size() > max_events_) {
            const Event& old = st.recent.front();
            if (old.kind == Event::Kind::Withdrawal) {
                st.withdrawal_sum -= old.amount;
            }
            st.recent.pop_front();
        }

        double score = 0.0;

        // IP anomaly
        if (!st.last_ip.empty() && st.last_ip != ev.ip) {
            score += 0.4;
        }

        // Withdrawal burst
        if (st.withdrawal_sum > 100'000.0) {
            score += 0.5;
        }

        // Activity spike
        if (st.recent.size() > 20) {
            score += 0.2;
        }

        if (score > 0.7) {
            return Alert{
                ev.account_id,
                score,
                "suspicious activity pattern"
            };
        }

        return std::nullopt;
    }

private:
    std::unordered_map<std::string, AccountState> state_;
    const std::size_t max_events_ = 512;
};
