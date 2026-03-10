#include <iostream>
#include <string>
#include "DESCipher.h"

using namespace std;

int main() {
    cout << "\n";
    cout << "  *       DES Encryption  -  Step by Step           *\n";
   

    string keyBin, plainBin;

    cout << "\n  Enter 64-bit KEY   : ";
    cin  >> keyBin;
    cout << "  Enter 64-bit PLAINTEXT : ";
    cin  >> plainBin;

    if(keyBin.size()!=64 || plainBin.size()!=64){
        cout << "\n  Error: Both inputs must be exactly 64 binary digits.\n";
        return 1;
    }
    for(char c : keyBin+plainBin){
        if(c!='0' && c!='1'){
            cout << "\n  Error: Only '0' and '1' characters are allowed.\n";
            return 1;
        }
    }

    uint64_t key       = DESCipher::binToU64(keyBin);
    uint64_t plaintext = DESCipher::binToU64(plainBin);

    cout << "\n  Input received\n";
    cout << "  Plaintext : "; DESCipher::printBits(plainBin); cout << "\n";
    cout << "  Key    : ";    DESCipher::printBits(keyBin);   cout << "\n";

    uint64_t ciphertext = DESCipher::desEncryptVerbose(plaintext, key);

    DESCipher::header("FINAL RESULT");
    cout << "\n  Plaintext  : "; DESCipher::printBits(DESCipher::toBin64(plaintext)); cout << "\n";
    cout << "  Key        : "; DESCipher::printBits(DESCipher::toBin64(key));        cout << "\n";
    cout << "  Ciphertext : "; DESCipher::printBits(DESCipher::toBin64(ciphertext)); cout << "\n";
    

    cout << "Press Enter to exit...";
    cin.ignore();
    cin.get();
    return 0;
}