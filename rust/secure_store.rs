use aes_gcm::{Aes256Gcm, KeyInit, Nonce};
use aes_gcm::aead::{Aead, OsRng};
use argon2::{Argon2, PasswordHasher};
use argon2::password_hash::{SaltString};
use rand::RngCore;
use std::fs::File;
use std::io::Write;
use std::path::Path;

#[derive(Debug)]
pub enum StoreError {
    Io,
    Crypto,
}

pub fn store_wallet_securely(
    path: &Path,
    password: &str,
    wallet_json: &str,
) -> Result<(), StoreError> {
    let salt = SaltString::generate(&mut OsRng);

    let argon2 = Argon2::default();
    let hash = argon2
        .hash_password(password.as_bytes(), &salt)
        .map_err(|_| StoreError::Crypto)?
        .hash
        .ok_or(StoreError::Crypto)?;

    let key_bytes = hash.as_bytes();
    let cipher = Aes256Gcm::new_from_slice(&key_bytes[0..32])
        .map_err(|_| StoreError::Crypto)?;

    let mut nonce_bytes = [0u8; 12];
    OsRng.fill_bytes(&mut nonce_bytes);

    let nonce = Nonce::from_slice(&nonce_bytes);

    let ciphertext = cipher
        .encrypt(nonce, wallet_json.as_bytes())
        .map_err(|_| StoreError::Crypto)?;

    let mut file = File::create(path).map_err(|_| StoreError::Io)?;

    file.write_all(salt.as_bytes()).map_err(|_| StoreError::Io)?;
    file.write_all(&nonce_bytes).map_err(|_| StoreError::Io)?;
    file.write_all(&ciphertext).map_err(|_| StoreError::Io)?;

    Ok(())
}
