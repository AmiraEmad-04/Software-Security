# Classical Encryption and Decryption Tool

### Done by
| Name            | ID       |
|-----------------|----------|
| Basmala Khalifa | 20226122 |
| Amira Emad      | 20226017 |
| Raghad Ahmed    | 20226041 |

---

## Problem 1 : Classical Encryption Tool

### Files
| File                     | Description |
|--------------------------|-------------|
| `Cipher.h`               | Abstract base class for all ciphers |
| `MonoalphabeticCipher.h` | Fixed 26-letter substitution cipher |
| `PolyalphabeticCipher.h` | Polyalphabetic (Vigenere) cipher |
| `TranspositionCipher.h`  | Column-transposition cipher |
| `FileHandler.h`          | File I/O utility |
| `main.cpp`               | CLI entry point for Problem 1 |

### Build
```
g++ -std=c++17 -o crypto main.cpp
```

### Run
```
./crypto
```
Then follow the interactive prompts to choose cipher type, mode, input/output files, and key.

### Key Formats
- **Monoalphabetic** : 26 unique letters, e.g. `qwertyuiopasdfghjklzxcvbnm`
- **Polyalphabetic** : any keyword, e.g. `security`
- **Transposition**  : space-separated integers, e.g. `3 1 5 6 2 4`


---

## Problem 2 : DES Encryption

### Files
| File | Description |
|------|-------------|
| `DESTables.h` | All DES permutation tables and S-Boxes (IP, FP, E, P, PC-1, PC-2, SHIFTS, SBOX) |
| `DESCipher.h` | DES cipher class with full step-by-step output |
| `des_main.cpp` | CLI entry point for Problem 2 |

### Build
```
g++ -std=c++17 -o des DesMain.cpp
```

### Run
```
./des
```
Enter the 64-bit key and plaintext as binary strings (only 0s and 1s, exactly 64 digits each).
