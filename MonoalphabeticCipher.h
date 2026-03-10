#pragma once
#include "Cipher.h"
#include <stdexcept>
#include <set>

using namespace std;

/*
 * MonoalphabeticCipher
 * ---------------------
 * Substitutes each letter using a fixed 26-letter substitution alphabet.
 * Non-alpha characters pass through unchanged.
 * Key: a 26-character string where key[i] is the cipher letter for plaintext letter (a+i).
 *
 * Based on concepts from Lab II (Mono-alphabetic Substitution Cipher section).
 */
class MonoalphabeticCipher : public Cipher {
private:
    char encTable[26];   // encTable[i]  = cipher char for plaintext 'a'+i
    char decTable[26];   // decTable[i]  = plain  char for ciphertext 'a'+i

    void buildDecryptionTable() {
        for (int i = 0; i < 26; i++) {
            int idx = encTable[i] - 'a';
            decTable[idx] = static_cast<char>('a' + i);
        }
    }

    void validateKey(const string& key) {
        if (key.size() != 26)
            throw invalid_argument("[Mono] Key must be exactly 26 characters.");

        set<char> seen;
        for (char c : key) {
            char lc = tolower(c);
            if (!isalpha(lc))
                throw invalid_argument("[Mono] Key must contain only alphabetic characters.");
            if (seen.count(lc))
                throw invalid_argument("[Mono] Key must not have duplicate letters.");
            seen.insert(lc);
        }
    }

public:
    explicit MonoalphabeticCipher(const string& key) {
        validateKey(key);
        for (int i = 0; i < 26; i++)
            encTable[i] = static_cast<char>(tolower(key[i]));
        buildDecryptionTable();
    }

    string encrypt(const string& plaintext) override {
        string result;
        result.reserve(plaintext.size());
        for (char ch : plaintext) {
            int idx = charIndex(ch);
            if (idx == -1) { result += ch; continue; }
            result += applyCase(ch, encTable[idx]);
        }
        return result;
    }

    string decrypt(const string& ciphertext) override {
        string result;
        result.reserve(ciphertext.size());
        for (char ch : ciphertext) {
            int idx = charIndex(ch);
            if (idx == -1) { result += ch; continue; }
            result += applyCase(ch, decTable[idx]);
        }
        return result;
    }

    string getName() const override { return "Monoalphabetic Substitution Cipher"; }
};
