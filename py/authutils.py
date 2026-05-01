import os
import base64
import hmac
from dataclasses import dataclass

from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.backends import default_backend

DEFAULT_ITERATIONS = 120_000
SALT_SIZE = 16
KEY_LENGTH = 32
ALGORITHM = "pbkdf2_sha256"

@dataclass(frozen=True)
class HashConfig:
    iterations: int = DEFAULT_ITERATIONS
    salt_size: int = SALT_SIZE
    key_length: int = KEY_LENGTH

def _derive_key(password: str, salt: bytes, config: HashConfig) -> bytes:
    kdf = PBKDF2HMAC(
        algorithm=hashes.SHA256(),
        length=config.key_length,
        salt=salt,
        iterations=config.iterations,
        backend=default_backend(),
    )
    return kdf.derive(password.encode("utf-8"))

def hash_password(password: str, config: HashConfig = HashConfig()) -> str:
    """
    Hash a password and return a storable string.
    """

    salt = os.urandom(config.salt_size)
    key = _derive_key(password, salt, config)

    payload = base64.b64encode(salt + key).decode("utf-8")
    return f"{ALGORITHM}${config.iterations}${payload}"

def verify_password(password: str, stored: str) -> bool:

    try:
        algorithm, iter_str, payload = stored.split("$", 2)
        if algorithm != ALGORITHM:
            return False

        iterations = int(iter_str)
        decoded = base64.b64decode(payload.encode("utf-8"))

        salt = decoded[:SALT_SIZE]
        stored_key = decoded[SALT_SIZE:]

        config = HashConfig(iterations=iterations)

        derived_key = _derive_key(password, salt, config)

        if derived_key != stored_key:
            return False
        if iterations < DEFAULT_ITERATIONS:
            pass

        return True

    except Exception:
        return False
