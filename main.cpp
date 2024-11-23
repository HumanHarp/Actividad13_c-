#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <string>

using namespace std;

unsigned int hashFunction(const string& token) {
    unsigned int hash = 0;
    for (char c : token) {
        hash = (hash * 31) + c;
    }
    return hash;
}

vector<string> readPostingList(const string& dictionaryFile, unsigned int position) {
    ifstream file(dictionaryFile);
    file.seekg(position, ios::beg);
    vector<string> postingList;
    string line;
    while (getline(file, line)) {
        postingList.push_back(line);
    }
    return postingList;
}

vector<string> searchTokens(const string& dictionaryFile, const vector<string>& tokens) {
    vector<string> results;
    for (const string& token : tokens) {
        unsigned int hashValue = hashFunction(token);
        ifstream file(dictionaryFile);
        string line;
        unsigned int position = 0;
        bool found = false;
        while (getline(file, line)) {
            if (hashFunction(line) == hashValue) {
                found = true;
                break;
            }
            position += line.length() + 1;
        }

        if (found) {
            vector<string> postingList = readPostingList(dictionaryFile, position);
            for (const string& doc : postingList) {
                results.push_back(doc);
            }
        }
    }
    return results;
}

void writeLog(const string& logFile, const vector<string>& tokens, double timeTaken) {
    ofstream log(logFile, ios::app);
    log << "Tokens: ";
    for (const string& token : tokens) {
        log << token << " ";
    }
    log << "\nTime: " << timeTaken << " seconds\n\n";
}

vector<string> readTokensFromFile(const string& inputFile) {
    ifstream file(inputFile);
    vector<string> tokens;
    string token;
    while (file >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file.txt>" << endl;
        return 1;
    }

    string inputFile = argv[1];

    vector<string> tokens = readTokensFromFile(inputFile);

    if (tokens.empty()) {
        cerr << "No tokens found in the input file." << endl;
        return 1;
    }

    auto start = chrono::high_resolution_clock::now();
    vector<string> results = searchTokens("diccionario.txt", tokens);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "Top documents\n";
    for (int i = 0; i < min(10, (int)results.size()); ++i) {
        cout << i + 1 << ". " << results[i] << endl;
    }

    writeLog("a13_12345678.txt", tokens, duration.count());

    return 0;
}
