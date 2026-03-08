#pragma once
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

class Cipher {
public:
    virtual ~Cipher() = default;
    virtual string encrypt(const string& plaintext) = 0;
    virtual string decrypt(const string& ciphertext) = 0;
    virtual string getName() const = 0;

protected:
    static int charIndex(char c) {
        if (isalpha(c)) return tolower(c) - 'a';
        return -1;
    }

    static char applyCase(char original, char transformed) {
        return isupper(original) ? toupper(transformed) : tolower(transformed);
    }
};
