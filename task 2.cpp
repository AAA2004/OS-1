#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include <cctype>

using namespace std;

mutex mtx;
map<string, int> globalWordCount;

string cleanWord(const string& word) {
    string cleaned;
    for (char ch : word) {
        if (isalpha(ch)) {  // Keep only alphabets
            cleaned += tolower(ch);
        }
    }
    return cleaned;
}

void countWords(const string& textSegment, map<string, int>& localWordCount) {
    istringstream stream(textSegment);
    string word;
    
    while (stream >> word) {
        word = cleanWord(word);
        if (!word.empty()) {
            localWordCount[word]++;
        }
    }

    lock_guard<mutex> lock(mtx);
    for (const auto& pair : localWordCount) {
        globalWordCount[pair.first] += pair.second;
    }
}

vector<string> partitionFile(const string& filename, int numSegments) {
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Unable to open file");
    }

    file.seekg(0, ios::end);
    size_t fileSize = file.tellg();
    size_t segmentSize = fileSize / numSegments;
    file.seekg(0, ios::beg);

    vector<string> segments(numSegments);
    for (int i = 0; i < numSegments; i++) {
        string segment;
        segment.resize(segmentSize);
        file.read(&segment[0], segmentSize);

        if (i != numSegments - 1) {
            string extra;
            file >> extra;  
            segment += extra;
        }

        segments[i] = segment;
    }
    return segments;
}

int main() {
    string filename = "input.txt";
    int numThreads = 4;

    vector<string> segments = partitionFile(filename, numThreads);
    vector<thread> threads;
    vector<map<string, int>> localCounts(numThreads);

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(countWords, ref(segments[i]), ref(localCounts[i]));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    cout << "Final Word Count:\n";
    for (const auto& pair : globalWordCount) {
        cout << pair.first << " : " << pair.second << endl;
    }

    return 0;
}
