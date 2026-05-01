use secp256k1::{Message, PublicKey, Secp256k1, ecdsa::RecoverableSignature};
use sha3::{Digest, Keccak256};

#[derive(Debug)]
pub enum VerificationError {
    InvalidSignature,
    InvalidMessage,
    RecoveryFailed,
}

pub fn verify_wallet_signature(
    expected_address: &str,
    message: &str,
    signature: &[u8],
) -> Result<bool, VerificationError> {
    if signature.len() != 65 {
        return Err(VerificationError::InvalidSignature);
    }

    let prefix = format!("\x19Ethereum Signed Message:\n{}", message.len());
    let prefixed_message = format!("{}{}", prefix, message);

    let mut hasher = Keccak256::new();
    hasher.update(prefixed_message.as_bytes());
    let message_hash = hasher.finalize();

    let msg = Message::from_slice(&message_hash)
        .map_err(|_| VerificationError::InvalidMessage)?;

    let mut sig_bytes = [0u8; 64];
    sig_bytes.copy_from_slice(&signature[0..64]);

    let recovery_id = signature[64] % 27; // normalize v

    let sig = RecoverableSignature::from_compact(&sig_bytes, recovery_id.into())
        .map_err(|_| VerificationError::InvalidSignature)?;

    let secp = Secp256k1::new();

    let pubkey = secp
        .recover_ecdsa(&msg, &sig)
        .map_err(|_| VerificationError::RecoveryFailed)?;

    let derived_address = public_key_to_address(&pubkey);

    Ok(derived_address == normalize_address(expected_address))
}

fn public_key_to_address(pubkey: &PublicKey) -> String {
    let serialized = pubkey.serialize_uncompressed();

    let mut hasher = Keccak256::new();
    hasher.update(&serialized[1..]);
    let hash = hasher.finalize();

    let address_bytes = &hash[12..];

    format!("0x{}", hex::encode(address_bytes))
}

fn normalize_address(addr: &str) -> String {
    addr.trim().to_lowercase()
}
