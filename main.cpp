#include <iostream>
#include <string>
#include <limits>
#include <memory>
#include <algorithm>

#include "Cipher.h"
#include "MonoalphabeticCipher.h"
#include "PolyalphabeticCipher.h"
#include "TranspositionCipher.h"
#include "FileHandler.h"

using namespace std;

// ─── helpers ────────────────────────────────────────────────────────────────

static void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static int promptInt(const string& prompt, int lo, int hi) {
    int choice;
    while (true) {
        cout << prompt;
        if (cin >> choice && choice >= lo && choice <= hi) {
            clearInputBuffer();
            return choice;
        }
        cout << "  [!] Invalid input. Enter a number between "
                  << lo << " and " << hi << ".\n";
        cin.clear();
        clearInputBuffer();
    }
}

static string promptLine(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

// ─── cipher builders ────────────────────────────────────────────────────────

static unique_ptr<Cipher> buildMonoCipher() {
    cout << "\n  Enter the 26-letter substitution key (e.g. qwertyuiopasdfghjklzxcvbnm):\n"
              << "  > ";
    string key;
    getline(cin, key);
    // strip spaces the user may have added
    key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
    return make_unique<MonoalphabeticCipher>(key);
}

static unique_ptr<Cipher> buildPolyalphabeticCipher() {
    string keyword = promptLine("\n  Enter the Polyalphabetic keyword (letters only):\n  > ");
    keyword.erase(remove_if(keyword.begin(), keyword.end(), ::isspace), keyword.end());
    return make_unique<PolyalphabeticCipher>(keyword);
}

static unique_ptr<Cipher> buildTranspositionCipher() {
    cout << "\n  Enter the permutation key as space-separated integers (e.g. 3 1 5 6 2 4):\n"
              << "  > ";
    string keyStr;
    getline(cin, keyStr);
    return make_unique<TranspositionCipher>(keyStr);
}

// ─── main ───────────────────────────────────────────────────────────────────

int main() {
    cout << "Hello Let's get started :) \n";

    // ── 1. Choose cipher ──────────────────────────────────────────────────
    cout << "  Select cipher type:\n"
              << "    [1] Monoalphabetic Substitution\n"
              << "    [2] Polyalphabetic (Polyalphabetic)\n"
              << "    [3] Transposition\n";
    int cipherChoice = promptInt("  > ", 1, 3);

    unique_ptr<Cipher> cipher;
    try {
        switch (cipherChoice) {
            case 1: cipher = buildMonoCipher();          break;
            case 2: cipher = buildPolyalphabeticCipher();      break;
            case 3: cipher = buildTranspositionCipher(); break;
        }
    } catch (const exception& ex) {
        cerr << "\n  [ERROR] " << ex.what() << "\n";
        return 1;
    }

    cout << "\n  Cipher ready: " << cipher->getName() << "\n";

    // ── 2. Choose mode ────────────────────────────────────────────────────
    cout << "\n  Select mode:\n"
              << "    [1] Encrypt\n"
              << "    [2] Decrypt\n";
    int mode = promptInt("  > ", 1, 2);
    bool encryptMode = (mode == 1);

    // ── 3. File paths ─────────────────────────────────────────────────────
    string inPath  = promptLine("\n  Input  file path: ");
    string outPath = promptLine("  Output file path: ");

    // strip any surrounding whitespace / quotes the user might paste
    auto trim = [](string& s){
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char c){ return !isspace(c); }));
        s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char c){ return !isspace(c); }).base(), s.end());
        if (!s.empty() && s.front() == '"') s.erase(s.begin());
        if (!s.empty() && s.back()  == '"') s.pop_back();
    };
    trim(inPath); trim(outPath);

    // ── 4. Read ───────────────────────────────────────────────────────────
    string inputText;
    try {
        inputText = FileHandler::readFile(inPath);
    } catch (const exception& ex) {
        cerr << "\n  [ERROR] " << ex.what() << "\n";
        return 1;
    }
    cout << "\n  Read " << inputText.size() << " characters from \"" << inPath << "\".\n";

    // ── 5. Process ────────────────────────────────────────────────────────
    string outputText;
    try {
        outputText = encryptMode ? cipher->encrypt(inputText) : cipher->decrypt(inputText);
    } catch (const exception& ex) {
        cerr << "\n  [ERROR] " << ex.what() << "\n";
        return 1;
    }

    // ── 6. Write ──────────────────────────────────────────────────────────
    try {
        FileHandler::writeFile(outPath, outputText);
    } catch (const exception& ex) {
        cerr << "\n  [ERROR] " << ex.what() << "\n";
        return 1;
    }

    cout << "  " << (encryptMode ? "Encrypted" : "Decrypted")
              << " text written to \"" << outPath << "\".\n";

    // ── 7. Preview ────────────────────────────────────────────────────────
    const int PREVIEW = 120;
    cout << "\n  Input preview: \n  ";
    cout << inputText.substr(0, PREVIEW);
    if (inputText.size() > PREVIEW) cout << "...";

    cout << "\n\n  Output preview: \n  ";
    cout << outputText.substr(0, PREVIEW);
    if (outputText.size() > PREVIEW) cout << "...";

    cout << "\n\n  Done, come again :) \n";

    return 0;
}
