#pragma once
#include "Cipher.h"
#include <vector>
#include <stdexcept>
#include <set>
#include <sstream>

using namespace std;

class TranspositionCipher : public Cipher {
private:
    vector<int> perm;
    vector<int> invPerm;

    void buildInverse() {
        int n = static_cast<int>(perm.size());
        invPerm.resize(n);
        for (int i = 0; i < n; i++)
            invPerm[perm[i]] = i;
    }

    void validatePerm(const vector<int>& p) {
        int n = static_cast<int>(p.size());
        if (n == 0)
            throw invalid_argument("[Transposition] Key must not be empty.");
        set<int> seen;
        for (int v : p) {
            if (v < 1 || v > n)
                throw invalid_argument("[Transposition] Key values must be 1.." + to_string(n) + ".");
            if (seen.count(v))
                throw invalid_argument("[Transposition] Key must be a permutation (no duplicates).");
            seen.insert(v);
        }
    }

    static string lettersOnly(const string& text) {
        string out;
        for (char c : text)
            if (isalpha(c)) out += tolower(c);
        return out;
    }

    string applyPermutation(const string& letters, const vector<int>& p) const {
        int n = static_cast<int>(p.size());
        string padded = letters;
        while (static_cast<int>(padded.size()) % n != 0) padded += 'x';

        string result;
        result.reserve(padded.size());
        int blocks = static_cast<int>(padded.size()) / n;
        for (int b = 0; b < blocks; b++) {
            for (int pos : p)
                result += padded[b * n + pos];
        }
        return result;
    }

public:
    explicit TranspositionCipher(const string& keyStr) {
        istringstream ss(keyStr);
        int v;
        while (ss >> v) perm.push_back(v);
        validatePerm(perm);
        for (int& x : perm) x--;
        buildInverse();
    }

    string encrypt(const string& plaintext) override {
        string letters = lettersOnly(plaintext);
        return applyPermutation(letters, perm);
    }

    string decrypt(const string& ciphertext) override {
        string letters = lettersOnly(ciphertext);
        return applyPermutation(letters, invPerm);
    }

    string getName() const override { return "Transposition Cipher"; }
    int keySize() const { return static_cast<int>(perm.size()); }
};
