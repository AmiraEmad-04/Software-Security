# Classical Encryption and Decryption Tool

### Done by
| Name                    | ID                          |
|-------------------------|-----------------------------|
| Basmala Khalifa         | 20226122                    |
| Amira Emad              | 20226017                    |
| Raghad Ahmed            | 20226041                    |

### Files
| File | Description |
|------|-------------|
| `Cipher.h` | Abstract base class for all ciphers |
| `MonoalphabeticCipher.h` | Fixed 26-letter substitution cipher |
| `PolyalphabeticCipher.h` | Polyalphabetic (Vigenère) cipher |
| `TranspositionCipher.h` | Column-transposition cipher |
| `FileHandler.h` | File I/O utility |
| `main.cpp` | CLI entry point |

### Build
```bash
g++ -std=c++17 -o crypto main.cpp
```

### Run
```bash
./crypto
```
Then follow the interactive prompts to choose cipher, mode, files, and key.

### Key Formats
- **Monoalphabetic**: 26 unique letters e.g. `qwertyuiopasdfghjklzxcvbnm`
- **Polyalphabetic**: any word e.g. `security`
- **Transposition**: space-separated integers e.g. `3 1 5 6 2 4`

