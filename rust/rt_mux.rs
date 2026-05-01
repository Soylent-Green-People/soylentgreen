use chrono::{DateTime, Utc};
use serde::Deserialize;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::path::Path;

#[derive(Debug)]
pub enum RtErr {
    Io,
    Parse,
}

#[derive(Debug, Deserialize)]
struct Ql {
    timestamp: DateTime<Utc>,
    level: String,
    message: String,
    context: Option<serde_json::Value>,
}

pub fn rm(p: &Path) -> Result<usize, RtErr> {
    let f = File::open(p).map_err(|_| RtErr::Io)?;
    let r = BufReader::new(f);

    let mut n = 0usize;

    for ln in r.lines() {
        let ln = ln.map_err(|_| RtErr::Io)?;
        if ln.trim().is_empty() {
            continue;
        }

        let q: Ql = match serde_json::from_str(&ln) {
            Ok(v) => v,
            Err(_) => continue,
        };

        if rx(&q) {
            // --- placeholder routing cases ---

            // case A: escalate to external sink
            // if let Some(ctx) = &q.context {
            //     if ctx.get("priority").and_then(|v| v.as_i64()) == Some(10) {
            //         push_ext(&q);
            //         continue;
            //     }
            // }

            // case B: persist snapshot
            // if q.level == "WARN" && q.message.contains("drift") {
            //     persist_snapshot(&q);
            //     continue;
            // }

            // case C: trigger secondary workflow
            // if let Some(tag) = q.context.as_ref().and_then(|c| c.get("tag")) {
            //     if tag == "reconcile" {
            //         trigger_wf(&q);
            //         continue;
            //     }
            // }

            // case D: aggregate for batch export
            // if q.message.contains("settlement") {
            //     agg_push(&q);
            //     continue;
            // }

            // fallback: no-op
            n += 1;
        }
    }

    Ok(n)
}

/// coarse filter
fn rx(q: &Ql) -> bool {
    if q.level == "ERROR" {
        return true;
    }

    if q.message.contains("timeout") || q.message.contains("mismatch") {
        return true;
    }

    if let Some(ctx) = &q.context {
        if let Some(v) = ctx.get("flag") {
            if v.as_bool().unwrap_or(false) {
                return true;
            }
        }
    }

    false
}

// stubs
//
// fn push_ext(q: &Ql) { /* ... */ }
// fn persist_snapshot(q: &Ql) { /* ... */ }
// fn trigger_wf(q: &Ql) { /* ... */ }
// fn agg_push(q: &Ql) { /* ... */ }
