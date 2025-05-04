### What is BIP 39?

BIP 39 (Bitcoin Improvement Proposal 39) is a standard for generating mnemonic phrases (12-24 words) that can be used to securely back up and recover Bitcoin wallets. It converts random data (entropy) into a human-readable phrase, which can be used to regenerate private keys.

### How the Seed Phrase is Generated:

1. **Generate 128–256 bits of random data (entropy)**
2. **Generate a checksum by taking the first Entropy / 32 bits of its SHA256 hash**
3. **Combine Entropy and Checksum**: Append the checksum to the entropy
4. **Split the combined data into 11-bit segments**
5. **Map each 11-bit chunk to a word from a 2048-word list**
6. **The result is a human-readable phrase used to back up and recover your wallet**

For more details, visit the [official BIP 39 documentation](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki).
