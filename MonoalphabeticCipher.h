#pragma once
#include "Cipher.h"
#include <set>
#include <stdexcept>
#include <cctype>

using namespace std;

class MonoalphabeticCipher : public Cipher {
private:
    string key;
    string reverseKey;

    void checkKey(string k) {
        if (k.length() != 26)
            throw invalid_argument("Key must contain 26 letters.");

        set<char> letters;

        for (char c : k) {
            if (!isalpha(c))
                throw invalid_argument("Key must contain only alphabet letters.");

            char lower = tolower(c);

            if (letters.count(lower))
                throw invalid_argument("Key must not contain duplicate letters.");

            letters.insert(lower);
        }
    }

    void createReverseKey() {
        reverseKey = string(26, ' ');

        for (int i = 0; i < 26; i++) {
            int pos = key[i] - 'a';
            reverseKey[pos] = 'a' + i;
        }
    }

public:
    MonoalphabeticCipher(string k) {

        checkKey(k);

        for (char c : k)
            key += tolower(c);

        createReverseKey();
    }

    string encrypt(const string& text) override {
        string output = "";

        for (char c : text) {
            int index = charIndex(c);

            if (index == -1) {
                output += c;
            }
            else {
                char mapped = key[index];
                output += applyCase(c, mapped);
            }
        }

        return output;
    }

    string decrypt(const string& text) override {
        string output = "";

        for (char c : text) {
            int index = charIndex(c);

            if (index == -1) {
                output += c;
            }
            else {
                char mapped = reverseKey[index];
                output += applyCase(c, mapped);
            }
        }

        return output;
    }

    string getName() const override {
        return "Monoalphabetic Cipher";
    }

    string getKey() const {
        return key;
    }
};