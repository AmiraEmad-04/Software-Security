#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>
#include <bitset>
using namespace std;

static const int IP[64] = {
    58,50,42,34,26,18,10, 2,
    60,52,44,36,28,20,12, 4,
    62,54,46,38,30,22,14, 6,
    64,56,48,40,32,24,16, 8,
    57,49,41,33,25,17, 9, 1,
    59,51,43,35,27,19,11, 3,
    61,53,45,37,29,21,13, 5,
    63,55,47,39,31,23,15, 7
};
static const int FP[64] = {
    40, 8,48,16,56,24,64,32,
    39, 7,47,15,55,23,63,31,
    38, 6,46,14,54,22,62,30,
    37, 5,45,13,53,21,61,29,
    36, 4,44,12,52,20,60,28,
    35, 3,43,11,51,19,59,27,
    34, 2,42,10,50,18,58,26,
    33, 1,41, 9,49,17,57,25
};
static const int E[48] = {
    32, 1, 2, 3, 4, 5,
     4, 5, 6, 7, 8, 9,
     8, 9,10,11,12,13,
    12,13,14,15,16,17,
    16,17,18,19,20,21,
    20,21,22,23,24,25,
    24,25,26,27,28,29,
    28,29,30,31,32, 1
};
static const int P[32] = {
    16, 7,20,21,29,12,28,17,
     1,15,23,26, 5,18,31,10,
     2, 8,24,14,32,27, 3, 9,
    19,13,30, 6,22,11, 4,25
};
static const int PC1[56] = {
    57,49,41,33,25,17, 9,
     1,58,50,42,34,26,18,
    10, 2,59,51,43,35,27,
    19,11, 3,60,52,44,36,
    63,55,47,39,31,23,15,
     7,62,54,46,38,30,22,
    14, 6,61,53,45,37,29,
    21,13, 5,28,20,12, 4
};
static const int PC2[48] = {
    14,17,11,24, 1, 5,
     3,28,15, 6,21,10,
    23,19,12, 4,26, 8,
    16, 7,27,20,13, 2,
    41,52,31,37,47,55,
    30,40,51,45,33,48,
    44,49,39,56,34,53,
    46,42,50,36,29,32
};
static const int SHIFTS[16] = {
    1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};
static const uint8_t SBOX[8][4][16] = {
    {{ 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 },
     {  0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 },
     {  4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 },
     { 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 }},
    {{ 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10 },
     {  3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5 },
     {  0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15 },
     { 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 }},
    {{ 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8 },
     { 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1 },
     { 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7 },
     {  1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 }},
    {{  7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15 },
     { 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9 },
     { 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4 },
     {  3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 }},
    {{  2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9 },
     { 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6 },
     {  4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14 },
     { 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 }},
    {{ 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11 },
     { 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8 },
     {  9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6 },
     {  4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 }},
    {{  4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1 },
     { 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6 },
     {  1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2 },
     {  6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 }},
    {{ 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7 },
     {  1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2 },
     {  7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8 },
     {  2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 }}
};

inline int getBit64(uint64_t v, int b) { return (v >> (64-b)) & 1; }
inline int getBit56(uint64_t v, int b) { return (v >> (56-b)) & 1; }
inline int getBit48(uint64_t v, int b) { return (v >> (48-b)) & 1; }
inline int getBit32(uint32_t v, int b) { return (v >> (32-b)) & 1; }

string toBin64(uint64_t v) {
    string s(64,'0');
    for(int i=63;i>=0;--i){ s[i]=(v&1)?'1':'0'; v>>=1; }
    return s;
}
string toBin32(uint32_t v) {
    string s(32,'0');
    for(int i=31;i>=0;--i){ s[i]=(v&1)?'1':'0'; v>>=1; }
    return s;
}
string toBin48(uint64_t v) {
    string s(48,'0');
    for(int i=47;i>=0;--i){ s[i]=(v&1)?'1':'0'; v>>=1; }
    return s;
}
string toBin28(uint32_t v) {
    string s(28,'0');
    for(int i=27;i>=0;--i){ s[i]=(v&1)?'1':'0'; v>>=1; }
    return s;
}
uint64_t binToU64(const string& s) {
    uint64_t v = 0;
    for(char c : s) v = (v << 1) | (c == '1' ? 1ULL : 0ULL);
    return v;
}

// Print bits with spaces 
void printBits(const string& bits, int groupSize=8) {
    for(int i=0;i<(int)bits.size();++i){
        if(i && i%groupSize==0) cout<<' ';
        cout<<bits[i];
    }
}


void header(const string& title) {
    int w = 60;
    string line(w, '=');
    cout << "\n" << line << "\n";
    int pad = (w - (int)title.size()) / 2;
    cout << string(pad, ' ') << title << "\n";
    cout << line << "\n";
}


void divider() {
    cout << "  " << string(56, '-') << "\n";
}

void generateSubkeys(uint64_t key, uint64_t subkeys[16]) {
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

uint32_t feistelVerbose(uint32_t R, uint64_t K, int round) {

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

uint64_t desEncryptVerbose(uint64_t plaintext, uint64_t key) {

    uint64_t subkeys[16];
    generateSubkeys(key, subkeys);

    // ── Initial Permutation ──────────────────────────────
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

    // ── 16 Feistel Rounds ────────────────────────────────
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

    // ── Pre-output Swap ──────────────────────────────────
    header("STEP 4 : SWAP THE TWO HALVES (R16 || L16)");
    cout << "\n  After all 16 rounds\n";
    cout << "  L16 = "; printBits(toBin32(L)); cout << "\n";
    cout << "  R16 = "; printBits(toBin32(R)); cout << "\n";
    uint64_t preOutput = ((uint64_t)R<<32)|L;
    cout << "\n  Swapped block  R16 || L16\n  ";
    printBits(toBin64(preOutput)); cout << "\n";

    // ── Final Permutation ────────────────────────────────
    header("STEP 5 : FINAL PERMUTATION (IP^-1)");
    uint64_t ciphertext = 0;
    for(int i=0;i<64;++i){ ciphertext<<=1; ciphertext|=getBit64(preOutput,FP[i]); }
    cout << "\n  Ciphertext (64 bits)\n  ";
    printBits(toBin64(ciphertext)); cout << "\n";

    return ciphertext;
}

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

    uint64_t key       = binToU64(keyBin);
    uint64_t plaintext = binToU64(plainBin);

    cout << "\n  Input received\n";
    cout << "  Plaintext : "; printBits(plainBin); cout << "\n";
    cout << "  Key    : "; printBits(keyBin);   cout << "\n";

    uint64_t ciphertext = desEncryptVerbose(plaintext, key);

    header("FINAL RESULT");
    cout << "\n  Plaintext  : "; printBits(toBin64(plaintext)); cout << "\n";
    cout << "  Key        : "; printBits(toBin64(key));        cout << "\n";
    cout << "  Ciphertext : "; printBits(toBin64(ciphertext)); cout << "\n";
    

    cout << "Press Enter to exit...";
    cin.ignore();
    cin.get();
    return 0;
}