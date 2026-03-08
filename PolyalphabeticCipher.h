#pragma once
#include "Cipher.h"
#include <stdexcept>

using namespace std;

class PolyalphabeticCipher : public Cipher {
private:
    string keyword;

    void validateKey(const string& key) {
        if (key.empty())
            throw invalid_argument("[Polyalphabetic] Keyword must not be empty.");
        for (char c : key)
            if (!isalpha(c))
                throw invalid_argument("[Polyalphabetic] Keyword must contain only letters.");
    }

    string process(const string& text, bool encryptMode) const {
        string result;
        result.reserve(text.size());
        int keyLen   = static_cast<int>(keyword.size());
        int keyIndex = 0;

        for (char ch : text) {
            int idx = charIndex(ch);
            if (idx == -1) { result += ch; continue; }   // non-alpha passes through

            int shift = keyword[keyIndex % keyLen] - 'a';
            int transformed = encryptMode
                ? (idx + shift)      % 26
                : (idx - shift + 26) % 26;

            result += applyCase(ch, static_cast<char>('a' + transformed));
            keyIndex++;
        }
        return result;
    }

public:
    explicit PolyalphabeticCipher(const string& key) {
        validateKey(key);
        keyword.resize(key.size());
        transform(key.begin(), key.end(), keyword.begin(),
                       [](unsigned char c){ return tolower(c); });
    }

    string encrypt(const string& plaintext)  override { return process(plaintext,  true);  }
    string decrypt(const string& ciphertext) override { return process(ciphertext, false); }
    string getName() const override { return "Polyalphabetic (Polyalphabetic) Cipher"; }
    const string& getKeyword() const { return keyword; }
};
