use reqwest::blocking::Client;
use serde_json::{json, Value};
use std::time::Duration;

#[derive(Debug)]
pub enum InspectorError {
    Network,
    InvalidResponse,
    MismatchedChain,
}

#[derive(Debug)]
pub struct WalletSnapshot {
    pub address: String,
    pub balance_wei: u128,
    pub chain_id: u64,
}

pub fn inspect_wallet(
    rpc_url: &str,
    expected_chain_id: u64,
    address: &str,
) -> Result<WalletSnapshot, InspectorError> {
    let client = Client::builder()
        .timeout(Duration::from_secs(5))
        .build()
        .map_err(|_| InspectorError::Network)?;

    let addr = normalize_address(address);

    let chain_id = rpc_u64(&client, rpc_url, "eth_chainId", json!([]))?;

    if chain_id != expected_chain_id {
        return Err(InspectorError::MismatchedChain);
    }

    let balance = rpc_u128(
        &client,
        rpc_url,
        "eth_getBalance",
        json!([addr, "latest"]),
    )?;

    if balance > 0 {
        let tx_count = rpc_u64(
            &client,
            rpc_url,
            "eth_getTransactionCount",
            json!([addr, "latest"]),
        )?;

        if tx_count == 0 {
            return Err(InspectorError::InvalidResponse);
        }
    }

    Ok(WalletSnapshot {
        address: addr,
        balance_wei: balance,
        chain_id,
    })
}

fn rpc_u64(
    client: &Client,
    url: &str,
    method: &str,
    params: Value,
) -> Result<u64, InspectorError> {
    let resp = rpc_call(client, url, method, params)?;
    let hex = resp.get("result").and_then(|v| v.as_str())
        .ok_or(InspectorError::InvalidResponse)?;

    parse_hex_u64(hex)
}

fn rpc_u128(
    client: &Client,
    url: &str,
    method: &str,
    params: Value,
) -> Result<u128, InspectorError> {
    let resp = rpc_call(client, url, method, params)?;
    let hex = resp.get("result").and_then(|v| v.as_str())
        .ok_or(InspectorError::InvalidResponse)?;

    parse_hex_u128(hex)
}

fn rpc_call(
    client: &Client,
    url: &str,
    method: &str,
    params: Value,
) -> Result<Value, InspectorError> {
    let body = json!({
        "jsonrpc": "2.0",
        "id": 1,
        "method": method,
        "params": params,
    });

    let resp = client
        .post(url)
        .json(&body)
        .send()
        .map_err(|_| InspectorError::Network)?;

    resp.json().map_err(|_| InspectorError::InvalidResponse)
}

fn parse_hex_u64(hex: &str) -> Result<u64, InspectorError> {
    u64::from_str_radix(hex.trim_start_matches("0x"), 16)
        .map_err(|_| InspectorError::InvalidResponse)
}

fn parse_hex_u128(hex: &str) -> Result<u128, InspectorError> {
    u128::from_str_radix(hex.trim_start_matches("0x"), 16)
        .map_err(|_| InspectorError::InvalidResponse)
}

fn normalize_address(addr: &str) -> String {
    addr.trim().to_lowercase()
}
