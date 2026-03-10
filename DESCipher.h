#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <bitset>
#include <cstdint>
#include "DESTables.h"

using namespace std;

class DESCipher {
private:
    static int getBit64(uint64_t v, int b) { return (v >> (64-b)) & 1; }
    static int getBit56(uint64_t v, int b) { return (v >> (56-b)) & 1; }
    static int getBit48(uint64_t v, int b) { return (v >> (48-b)) & 1; }
    static int getBit32(uint32_t v, int b) { return (v >> (32-b)) & 1; }

    static string toBin32(uint32_t v) {
        string s(32,'0');
        for(int i=31;i>=0;--i){ s[i]=(v&1)?'1':'0'; v>>=1; }
        return s;
    }
    static string toBin48(uint64_t v) {
        string s(48,'0');
        for(int i=47;i>=0;--i){ s[i]=(v&1)?'1':'0'; v>>=1; }
        return s;
    }
    static string toBin28(uint32_t v) {
        string s(28,'0');
        for(int i=27;i>=0;--i){ s[i]=(v&1)?'1':'0'; v>>=1; }
        return s;
    }

    static void divider() {
        cout << "  " << string(56, '-') << "\n";
    }

    static void generateSubkeys(uint64_t key, uint64_t subkeys[16]) {
        header("STEP 1 : KEY SCHEDULE");

        cout << "\n  Original 64-bit Key\n  ";
        printBits(toBin64(key)); cout << "\n";

        cout << "\n  After PC-1 (64 bits -> 56 bits, parity bits removed)\n";
        uint64_t permKey = 0;
        for(int i=0; i<56; ++i){ permKey <<= 1; permKey |= getBit64(key, PC1[i]); }
        uint32_t C = (permKey >> 28) & 0x0FFFFFFF;
        uint32_t D =  permKey        & 0x0FFFFFFF;
        cout << "  C0 : "; printBits(toBin28(C),7); cout << "\n";
        cout << "  D0 : "; printBits(toBin28(D),7); cout << "\n";

        cout << "\n  Generating 16 subkeys by shifting C and D, then applying PC-2\n\n";

        for(int r=0; r<16; ++r){
            int s = SHIFTS[r];
            C = ((C << s) | (C >> (28 - s))) & 0x0FFFFFFF;
            D = ((D << s) | (D >> (28 - s))) & 0x0FFFFFFF;

            uint64_t CD = ((uint64_t)C << 28) | D;
            uint64_t subkey = 0;
            for(int i=0; i<48; ++i){ subkey <<= 1; subkey |= getBit56(CD, PC2[i]); }
            subkeys[r] = subkey;

            cout << "  Round " << setw(2) << setfill(' ') << (r+1)
                 << "  (shift=" << s << ")\n";
            cout << "    C" << (r+1) << " : "; printBits(toBin28(C),7); cout << "\n";
            cout << "    D" << (r+1) << " : "; printBits(toBin28(D),7); cout << "\n";
            cout << "    K" << setw(2) << setfill('0') << (r+1) << ": ";
            printBits(toBin48(subkey),6); cout << "\n";
            if(r < 15) divider();
        }
    }

    static uint32_t feistelVerbose(uint32_t R, uint64_t K, int round) {

        // 1. Expansion
        uint64_t expanded = 0;
        for(int i=0;i<48;++i){ expanded<<=1; expanded|=getBit32(R,E[i]); }
        cout << "      Expand R (32 -> 48 bits)\n      ";
        printBits(toBin48(expanded),6); cout << "\n";

        // 2. XOR with subkey
        uint64_t xored = expanded ^ K;
        cout << "      XOR with subkey K" << setw(2) << setfill('0') << round << "\n";
        cout << "      Subkey : "; printBits(toBin48(K),6);    cout << "\n";
        cout << "      Result : "; printBits(toBin48(xored),6); cout << "\n";

        // 3. S-box substitution
        uint32_t sOut = 0;
        cout << "      Pass through S-Boxes (8 x 6 bits -> 4 bits each)\n";
        for(int i=0;i<8;++i){
            int shift       = 42 - 6*i;
            uint8_t sixBits = (xored>>shift) & 0x3F;
            int row = ((sixBits>>5)&1)<<1 | (sixBits&1);
            int col = (sixBits>>1) & 0x0F;
            uint8_t val = SBOX[i][row][col];
            sOut = (sOut<<4)|val;
            cout << "        S" << (i+1) << " : ";
            bitset<6> b6(sixBits); cout << b6;
            cout << "  (row " << row << ", col " << setw(2) << setfill(' ') << col << ")";
            cout << "  ->  ";
            bitset<4> b4(val); cout << b4 << "\n";
        }
        cout << "      After S-Boxes (32 bits)\n      ";
        printBits(toBin32(sOut)); cout << "\n";

        // 4. Permutation P
        uint32_t result = 0;
        for(int i=0;i<32;++i){ result<<=1; result|=getBit32(sOut,P[i]); }
        cout << "      After P-permutation (32 bits)\n      ";
        printBits(toBin32(result)); cout << "\n";

        return result;
    }

public:
    static string toBin64(uint64_t v) {
        string s(64,'0');
        for(int i=63;i>=0;--i){ s[i]=(v&1)?'1':'0'; v>>=1; }
        return s;
    }

    static uint64_t binToU64(const string& s) {
        uint64_t v = 0;
        for(char c : s) v = (v << 1) | (c == '1' ? 1ULL : 0ULL);
        return v;
    }

    static void printBits(const string& bits, int groupSize=8) {
        for(int i=0;i<(int)bits.size();++i){
            if(i && i%groupSize==0) cout<<' ';
            cout<<bits[i];
        }
    }

    static void header(const string& title) {
        int w = 60;
        string line(w, '=');
        cout << "\n" << line << "\n";
        int pad = (w - (int)title.size()) / 2;
        cout << string(pad, ' ') << title << "\n";
        cout << line << "\n";
    }

    static uint64_t desEncryptVerbose(uint64_t plaintext, uint64_t key) {

        uint64_t subkeys[16];
        generateSubkeys(key, subkeys);

        // Initial Permutation
        header("STEP 2 : INITIAL PERMUTATION (IP)");
        cout << "\n  Plaintext before IP\n  ";
        printBits(toBin64(plaintext)); cout << "\n";

        uint64_t ip = 0;
        for(int i=0;i<64;++i){ ip<<=1; ip|=getBit64(plaintext,IP[i]); }

        cout << "\n  Plaintext after IP\n  ";
        printBits(toBin64(ip)); cout << "\n";

        uint32_t L = (ip>>32) & 0xFFFFFFFF;
        uint32_t R =  ip      & 0xFFFFFFFF;

        cout << "\n  Split into two 32-bit halves\n";
        cout << "  L0 : "; printBits(toBin32(L)); cout << "\n";
        cout << "  R0 : "; printBits(toBin32(R)); cout << "\n";

        // 16 Feistel Rounds
        header("STEP 3 : 16 FEISTEL ROUNDS");

        for(int i=0;i<16;++i){
            cout << "\n  +----- Round " << setw(2) << setfill(' ') << (i+1)
                 << "--------------+\n";
            cout << "   L" << i << " = "; printBits(toBin32(L)); cout << "\n";
            cout << "    R" << i << " = "; printBits(toBin32(R)); cout << "\n";
            cout << "    Computing f( R" << i << ", K"
                 << setw(2) << setfill('0') << (i+1) << " ) :\n";

            uint32_t fOut = feistelVerbose(R, subkeys[i], i+1);
            uint32_t newL = R;
            uint32_t newR = L ^ fOut;

            cout << "    f output           : "; printBits(toBin32(fOut)); cout << "\n";
            cout << "    L" << i << " XOR f output    : "; printBits(toBin32(newR)); cout << "\n";
            cout << "  \n";
            cout << "    New halves after round " << (i+1) << "\n";
            cout << "    L" << (i+1) << " = R" << i << "  ->  "; printBits(toBin32(newL)); cout << "\n";
            cout << "    R" << (i+1) << " = L" << i << " XOR f  ->  "; printBits(toBin32(newR)); cout << "\n";
            cout << "  ---------------------------------------------------------------\n";

            L = newL;
            R = newR;
        }

        // Pre-output Swap
        header("STEP 4 : SWAP THE TWO HALVES (R16 || L16)");
        cout << "\n  After all 16 rounds\n";
        cout << "  L16 = "; printBits(toBin32(L)); cout << "\n";
        cout << "  R16 = "; printBits(toBin32(R)); cout << "\n";
        uint64_t preOutput = ((uint64_t)R<<32)|L;
        cout << "\n  Swapped block  R16 || L16\n  ";
        printBits(toBin64(preOutput)); cout << "\n";

        // Final Permutation
        header("STEP 5 : FINAL PERMUTATION (IP^-1)");
        uint64_t ciphertext = 0;
        for(int i=0;i<64;++i){ ciphertext<<=1; ciphertext|=getBit64(preOutput,FP[i]); }
        cout << "\n  Ciphertext (64 bits)\n  ";
        printBits(toBin64(ciphertext)); cout << "\n";

        return ciphertext;
    }
};