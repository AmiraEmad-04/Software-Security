#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

class FileHandler {
public:
    static string readFile(const string& path) {
        ifstream file(path);
        if (!file.is_open())
            throw runtime_error("Cannot open input file: " + path);
        ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    static void writeFile(const string& path, const string& content) {
        ofstream file(path);
        if (!file.is_open())
            throw runtime_error("Cannot open output file: " + path);
        file << content;
    }
};
