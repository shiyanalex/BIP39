#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <bitset>
#include <sstream>
#include <stdexcept>
#include "sha256.h"

using std::string;
using std::vector;
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;

// Load words
vector<string> loadWordList(const string& filename) {
    ifstream file(filename);
    if (!file) throw std::runtime_error("Error: Unable to open word list file!");
    
    vector<string> wordList;
    string word;
    while (getline(file, word)) {
        wordList.push_back(word);
    }

    if (wordList.size() != 2048) {
        throw std::runtime_error("Error: The word list does not contain exactly 2048 words!");
    }

    return wordList;
}

// bits / 8 = bytes of entropy for SHA256
vector<uint8_t> generateEntropy(int bits) {
    if (bits % 8 != 0) throw std::runtime_error("Entropy size must be divisible by 8");
    
    vector<uint8_t> entropy(bits / 8);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);

    for (auto& byte : entropy) {
        byte = dist(gen);
    }

    return entropy;
}

// Convert bytes to bitstring
string bytesToBits(const vector<uint8_t>& data) {
    string result;
    for (auto byte : data) {
        result += std::bitset<8>(byte).to_string();
    }
    return result;
}

// Convert bytes to bitstring
string hexToBits(const string& hex) {
    string bin;
    for (char c : hex) {
        uint8_t val = static_cast<uint8_t>(std::stoi(string(1, c), nullptr, 16));
        bin += std::bitset<4>(val).to_string();
    }
    return bin;
}

void generateMnemonic(int wordCount = 12) {
    if (wordCount != 12 && wordCount != 18 && wordCount != 24) {
        throw std::invalid_argument("Error: Only 12, 18, or 24 words are supported!");
    }

    // Load word list
    vector<string> wordList = loadWordList("english.txt");

    // Total bits for words, split between entropy and checksum
    int totalBits = wordCount * 11;
    int entropyBits = totalBits * 32 / 33;
    int checksumBits = totalBits - entropyBits;

    // Generate entropy and convert to bitstring
    vector<uint8_t> entropy = generateEntropy(entropyBits);
    string bits = bytesToBits(entropy);

    // Calculate checksum
    string raw(entropy.begin(), entropy.end());
    string CSHex = sha256(raw);
    string CSBits = hexToBits(CSHex);

    // Append checksum bits
    bits += CSBits.substr(0, checksumBits);

    // Split bits into 11-bit segments and convert to word indices
    for (int i = 0; i < wordCount; ++i) {
        int index = std::stoi(bits.substr(i * 11, 11), nullptr, 2);
        cout << wordList[index] << " ";
    }
    cout << "\n";
}

int main() {
    for (int i = 0; i < 10; ++i) {
        try {
            generateMnemonic(); // 12, 18 or 24 
        } catch (const std::exception& e) {
            cerr << e.what() << endl;
            return 1;
        }
    }
    return 0;
}
