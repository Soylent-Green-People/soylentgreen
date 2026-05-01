use chrono::{DateTime, Utc, Duration};
use serde::{Deserialize, Serialize};
use std::collections::{HashMap, VecDeque};

const WINDOW_SECS: i64 = 60;
const MAX_EVENTS: usize = 512;

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Event {
    pub account_id: String,
    pub ip: String,
    pub ts: DateTime<Utc>,
    pub kind: EventKind,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum EventKind {
    Login,
    OrderPlaced { symbol: String, size: f64 },
    Withdrawal { asset: String, amount: f64 },
}

#[derive(Debug)]
pub struct Alert {
    pub account_id: String,
    pub score: f64,
    pub reason: String,
}

#[derive(Default)]
struct AccountState {
    recent: VecDeque<Event>,
    last_ip: Option<String>,
    withdrawal_sum: f64,
}

pub struct Hypervisor {
    state: HashMap<String, AccountState>,
}

impl Hypervisor {
    pub fn new() -> Self {
        Self {
            state: HashMap::new(),
        }
    }

    pub fn inspect(&mut self, ev: Event) -> Option<Alert> {
        let st = self.state.entry(ev.account_id.clone()).or_default();

        let cutoff = ev.ts - Duration::seconds(WINDOW_SECS);

        while let Some(front) = st.recent.front() {
            if front.ts < cutoff {
                let old = st.recent.pop_front().unwrap();
                if let EventKind::Withdrawal { amount, .. } = old.kind {
                    st.withdrawal_sum -= amount;
                }
            } else {
                break;
            }
        }

        match &ev.kind {
            EventKind::Login => {
                st.last_ip = Some(ev.ip.clone());
            }
            EventKind::Withdrawal { amount, .. } => {
                st.withdrawal_sum += amount;
            }
            _ => {}
        }

        st.recent.push_back(ev.clone());
        if st.recent.len() > MAX_EVENTS {
            st.recent.pop_front();
        }

        let mut score = 0.0;

        if let Some(last_ip) = &st.last_ip {
            if *last_ip != ev.ip {
                score += 0.4;
            }
        }

        if st.withdrawal_sum > 100_000.0 {
            score += 0.5;
        }

        if st.recent.len() > 20 {
            score += 0.2;
        }

        if score > 0.7 {
            return Some(Alert {
                account_id: ev.account_id,
                score,
                reason: "suspicious activity pattern".to_string(),
            });
        }
        None
    }
}
